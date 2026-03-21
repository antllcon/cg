#include "OpenglRenderer.h"
#include "src/system/AppConfig.h"
#include <libs/glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <numbers>
#include <stdexcept>

#define STB_TRUETYPE_IMPLEMENTATION
#pragma warning(push, 0)
#include <ranges>
#include <stb_truetype.h>
#pragma warning(pop)

struct FontData
{
	uint32_t textureId{0};
	float fontSize{0.0f};
	std::vector<stbtt_packedchar> asciiChars;
	std::vector<stbtt_packedchar> cyrillicChars;
};

namespace
{
constexpr float CIRCLE_SEGMENTS = 64.0f;
constexpr int FLOATS_PER_VERTEX = 8;
constexpr int ATLAS_SIZE = 1024;
constexpr int CORNER_SEGMENTS = 16;

void AssertIsGladInitialized(int result)
{
	if (!result)
	{
		throw std::runtime_error("Не удалось инициализировать GLAD");
	}
}

void AssertIsFileExists(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Файл шрифта не найден");
	}
}

void AssertIsFontLoaded(bool result)
{
	if (!result)
	{
		throw std::runtime_error("Не удалось загрузить или инициализировать шрифт");
	}
}

void AssertIsFontExists(bool result)
{
	if (!result)
	{
		throw std::runtime_error("Шрифт с указанным ID не найден");
	}
}

void PushVertex(std::vector<float>& data, float x, float y, const Color& color, float u = 0.0f, float v = 0.0f)
{
	data.push_back(x);
	data.push_back(y);
	data.push_back(color.GetR());
	data.push_back(color.GetG());
	data.push_back(color.GetB());
	data.push_back(color.GetA());
	data.push_back(u);
	data.push_back(v);
}

void GenerateEllipseFill(std::vector<float>& data, const Point2f& center, const Point2f& radius, const Color& color)
{
	for (float i = 0.0f; i < CIRCLE_SEGMENTS; ++i)
	{
		float theta1 = 2.0f * std::numbers::pi_v<float> * i / CIRCLE_SEGMENTS;
		float theta2 = 2.0f * std::numbers::pi_v<float> * (i + 1.0f) / CIRCLE_SEGMENTS;

		PushVertex(data, center.x, center.y, color);
		PushVertex(data, center.x + radius.x * std::cos(theta1), center.y + radius.y * std::sin(theta1), color);
		PushVertex(data, center.x + radius.x * std::cos(theta2), center.y + radius.y * std::sin(theta2), color);
	}
}

void GenerateEllipseOutline(std::vector<float>& data, const Point2f& center, const Point2f& radius, float thickness, const Color& color)
{
	for (float i = 0.0f; i < CIRCLE_SEGMENTS; ++i)
	{
		float theta1 = 2.0f * std::numbers::pi_v<float> * i / CIRCLE_SEGMENTS;
		float theta2 = 2.0f * std::numbers::pi_v<float> * (i + 1.0f) / CIRCLE_SEGMENTS;

		float cos1 = std::cos(theta1);
		float sin1 = std::sin(theta1);
		float x1Inner = center.x + radius.x * cos1;
		float y1Inner = center.y + radius.y * sin1;
		float x1Outer = center.x + (radius.x + thickness) * cos1;
		float y1Outer = center.y + (radius.y + thickness) * sin1;

		float cos2 = std::cos(theta2);
		float sin2 = std::sin(theta2);
		float x2Inner = center.x + radius.x * cos2;
		float y2Inner = center.y + radius.y * sin2;
		float x2Outer = center.x + (radius.x + thickness) * cos2;
		float y2Outer = center.y + (radius.y + thickness) * sin2;

		PushVertex(data, x1Inner, y1Inner, color);
		PushVertex(data, x1Outer, y1Outer, color);
		PushVertex(data, x2Outer, y2Outer, color);

		PushVertex(data, x1Inner, y1Inner, color);
		PushVertex(data, x2Outer, y2Outer, color);
		PushVertex(data, x2Inner, y2Inner, color);
	}
}

uint32_t DecodeUtf8(const char*& text, const char* end)
{
	if (text >= end) return 0;

	uint8_t c0 = static_cast<uint8_t>(*text++);
	if (c0 < 0x80) return c0;

	if (text >= end) return 0;
	uint8_t c1 = static_cast<uint8_t>(*text++);
	if (c0 < 0xE0) return (c0 & 0x1F) << 6 | c1 & 0x3F;

	if (text >= end) return 0;
	uint8_t c2 = static_cast<uint8_t>(*text++);
	if (c0 < 0xF0) return (c0 & 0x0F) << 12 | (c1 & 0x3F) << 6 | c2 & 0x3F;

	if (text >= end) return 0;
	uint8_t c3 = static_cast<uint8_t>(*text++);
	return (c0 & 0x07) << 18 | (c1 & 0x3F) << 12 | (c2 & 0x3F) << 6 | c3 & 0x3F;
}

void GenerateArcPoints(std::vector<Point2f>& points, const Point2f& center, float radius, float startAngle, float endAngle)
{
	for (int i = 0; i <= CORNER_SEGMENTS; ++i)
	{
		float fraction = static_cast<float>(i) / CORNER_SEGMENTS;
		float angle = startAngle + fraction * (endAngle - startAngle);
		float x = center.x + radius * std::cos(angle);
		float y = center.y + radius * std::sin(angle);
		points.push_back({x, y});
	}
}
} // namespace

OpenglRenderer::OpenglRenderer()
	: m_shaderProgram(0)
	, m_vao(0)
	, m_vbo(0)
	, m_projectionLocation(0)
	, m_renderModeLocation(0)
	, m_textureLocation(0)
	, m_nextFontId(1)
{
	AssertIsGladInitialized(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	m_shaderProgram = CreateShaderProgram();
	m_projectionLocation = glGetUniformLocation(m_shaderProgram, "projection");
	m_renderModeLocation = glGetUniformLocation(m_shaderProgram, "uRenderMode");
	m_textureLocation = glGetUniformLocation(m_shaderProgram, "uTexture");

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	PushTransform();
}

OpenglRenderer::~OpenglRenderer()
{
	for (const auto& font : m_fonts | std::views::values)
	{
		glDeleteTextures(1, &font->textureId);
	}

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteProgram(m_shaderProgram);
}

void OpenglRenderer::Clear(const Color& color)
{
	glClearColor(color.GetR(), color.GetG(), color.GetB(), color.GetA());
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenglRenderer::Display()
{
	GLFWwindow* window = glfwGetCurrentContext();
	if (window)
	{
		glfwSwapBuffers(window);
	}
}

void OpenglRenderer::PushTransform()
{
	if (m_transformStack.empty())
	{
		glm::mat4 ortho = glm::ortho(
			0.0f,
			AppConfig::WINDOW_WIDTH,
			AppConfig::WINDOW_HEIGHT,
			0.0f,
			-1.0f,
			1.0f);
		m_transformStack.push_back(ortho);
	}
	else
	{
		m_transformStack.push_back(m_transformStack.back());
	}
}

void OpenglRenderer::PopTransform()
{
	if (m_transformStack.size() > 1)
	{
		m_transformStack.pop_back();
	}
}

void OpenglRenderer::SetClipRect(const Point2f& position, const Point2f& size)
{
	if (size.x <= 0.0f || size.y <= 0.0f)
	{
		glDisable(GL_SCISSOR_TEST);
		return;
	}

	glEnable(GL_SCISSOR_TEST);

	GLint windowHeight = static_cast<GLint>(AppConfig::WINDOW_HEIGHT);
	GLint scissorY = windowHeight - static_cast<GLint>(position.y + size.y);

	glScissor(
		static_cast<GLint>(position.x),
		scissorY,
		static_cast<GLsizei>(size.x),
		static_cast<GLsizei>(size.y));
}

uint32_t OpenglRenderer::LoadFont(const std::filesystem::path& path, float fontSize)
{
	AssertIsFileExists(path);

	std::ifstream file(path, std::ios::binary | std::ios::ate);
	AssertIsFontLoaded(file.is_open());

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> ttfBuffer(size);
	file.read(reinterpret_cast<char*>(ttfBuffer.data()), size);

	std::vector<unsigned char> pixels(ATLAS_SIZE * ATLAS_SIZE);

	auto fontData = std::make_unique<FontData>();
	fontData->fontSize = fontSize;
	fontData->asciiChars.resize(96);
	fontData->cyrillicChars.resize(256);

	stbtt_pack_context pc;
	AssertIsFontLoaded(stbtt_PackBegin(&pc, pixels.data(), ATLAS_SIZE, ATLAS_SIZE, 0, 1, nullptr));

	stbtt_pack_range ranges[2]{};

	ranges[0].font_size = fontSize;
	ranges[0].first_unicode_codepoint_in_range = 32;
	ranges[0].num_chars = 96;
	ranges[0].chardata_for_range = fontData->asciiChars.data();

	ranges[1].font_size = fontSize;
	ranges[1].first_unicode_codepoint_in_range = 0x0400;
	ranges[1].num_chars = 256;
	ranges[1].chardata_for_range = fontData->cyrillicChars.data();

	stbtt_PackSetOversampling(&pc, 2, 2);
	AssertIsFontLoaded(stbtt_PackFontRanges(&pc, ttfBuffer.data(), 0, ranges, 2));
	stbtt_PackEnd(&pc);

	glGenTextures(1, &fontData->textureId);
	glBindTexture(GL_TEXTURE_2D, fontData->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_SIZE, ATLAS_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	uint32_t fontId = m_nextFontId++;
	m_fonts[fontId] = std::move(fontData);

	return fontId;
}

void OpenglRenderer::DrawTextData(const Point2f& position, std::string_view text, uint32_t fontId, const Color& color)
{
	AssertIsFontExists(m_fonts.contains(fontId));
	const auto& font = m_fonts[fontId];

	std::vector<float> data;
	float currentX = position.x;
	float currentY = position.y;

	const char* textPtr = text.data();
	const char* textEnd = textPtr + text.size();

	while (textPtr < textEnd)
	{
		uint32_t codepoint = DecodeUtf8(textPtr, textEnd);
		if (codepoint == 0) break;

		stbtt_aligned_quad quad;

		if (codepoint >= 32 && codepoint < 128)
		{
			stbtt_GetPackedQuad(font->asciiChars.data(), ATLAS_SIZE, ATLAS_SIZE, codepoint - 32, &currentX, &currentY, &quad, 1);
		}
		else if (codepoint >= 0x0400 && codepoint < 0x0500)
		{
			stbtt_GetPackedQuad(font->cyrillicChars.data(), ATLAS_SIZE, ATLAS_SIZE, codepoint - 0x0400, &currentX, &currentY, &quad, 1);
		}
		else
		{
			continue;
		}

		PushVertex(data, quad.x0, quad.y0, color, quad.s0, quad.t0);
		PushVertex(data, quad.x1, quad.y0, color, quad.s1, quad.t0);
		PushVertex(data, quad.x0, quad.y1, color, quad.s0, quad.t1);

		PushVertex(data, quad.x1, quad.y0, color, quad.s1, quad.t0);
		PushVertex(data, quad.x1, quad.y1, color, quad.s1, quad.t1);
		PushVertex(data, quad.x0, quad.y1, color, quad.s0, quad.t1);
	}

	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_TRIANGLES, 1, font->textureId);
}

void OpenglRenderer::DrawPoint(const Point2f& position, const Color& color)
{
	std::vector<float> data;
	PushVertex(data, position.x, position.y, color);
	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_POINTS);
}

void OpenglRenderer::DrawLine(const Point2f& start, const Point2f& end, const Color& color, float thickness)
{
	glLineWidth(thickness);
	std::vector<float> data;
	PushVertex(data, start.x, start.y, color);
	PushVertex(data, end.x, end.y, color);
	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_LINES);
	glLineWidth(1.0f);
}

void OpenglRenderer::DrawRect(const Point2f& position, const Point2f& size, const RenderStyle& style)
{
	std::vector<float> data;

	PushVertex(data, position.x, position.y, style.fillColor);
	PushVertex(data, position.x + size.x, position.y, style.fillColor);
	PushVertex(data, position.x, position.y + size.y, style.fillColor);

	PushVertex(data, position.x + size.x, position.y, style.fillColor);
	PushVertex(data, position.x + size.x, position.y + size.y, style.fillColor);
	PushVertex(data, position.x, position.y + size.y, style.fillColor);

	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_TRIANGLES);
}

void OpenglRenderer::DrawRoundedRect(const Point2f& position, const Point2f& size, float radius, const RenderStyle& style)
{
	// TODO: исправить, это временное решение через DrawPolygon + окружности
	radius = std::min(radius, std::min(size.x, size.y) / 2.0f);

	if (radius <= 0.0f)
	{
		DrawRect(position, size, style);
		return;
	}

	std::vector<Point2f> points;
	points.reserve(CORNER_SEGMENTS * 4 + 4);

	float w = size.x;
	float h = size.y;
	float x = position.x;
	float y = position.y;
	float pi = std::numbers::pi_v<float>;

	GenerateArcPoints(points, {x + w - radius, y + radius}, radius, 1.5f * pi, 2.0f * pi);
	GenerateArcPoints(points, {x + w - radius, y + h - radius}, radius, 0.0f, 0.5f * pi);
	GenerateArcPoints(points, {x + radius, y + h - radius}, radius, 0.5f * pi, pi);
	GenerateArcPoints(points, {x + radius, y + radius}, radius, pi, 1.5f * pi);

	DrawPolygon(points, style);
}

void OpenglRenderer::DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style)
{
	std::vector<float> fillData;
	GenerateEllipseFill(fillData, center, radius, style.fillColor);
	RenderGeometry(fillData.data(), fillData.size() / FLOATS_PER_VERTEX, GL_TRIANGLES);

	if (style.outlineThickness > 0.0f)
	{
		std::vector<float> outlineData;
		GenerateEllipseOutline(outlineData, center, radius, style.outlineThickness, style.outlineColor);
		RenderGeometry(outlineData.data(), outlineData.size() / FLOATS_PER_VERTEX, GL_TRIANGLES);
	}
}

void OpenglRenderer::DrawPolygon(const std::vector<Point2f>& points, const RenderStyle& style)
{
	if (points.size() < 3)
	{
		return;
	}

	std::vector<float> data;
	for (size_t i = 1; i < points.size() - 1; ++i)
	{
		PushVertex(data, points[0].x, points[0].y, style.fillColor);
		PushVertex(data, points[i].x, points[i].y, style.fillColor);
		PushVertex(data, points[i + 1].x, points[i + 1].y, style.fillColor);
	}

	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_TRIANGLES);
}

void OpenglRenderer::DrawPolyline(const std::vector<Point2f>& points, const Color& color, float thickness)
{
	if (points.size() < 2)
	{
		return;
	}

	glLineWidth(thickness);
	std::vector<float> data;
	for (const auto& point : points)
	{
		PushVertex(data, point.x, point.y, color);
	}

	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_LINE_STRIP);
	glLineWidth(1.0f);
}

void OpenglRenderer::DrawBezier(const Point2f&, const Point2f&, const Point2f&, const Point2f&, const Color&, float)
{
}

void OpenglRenderer::DrawTexture(const Point2f&, const Point2f&, const ITexture*)
{
}

void OpenglRenderer::DrawTexturePart(const Point2f&, const Point2f&, const Point2f&, const Point2f&, const ITexture*)
{
}

void OpenglRenderer::RenderGeometry(const float* data, size_t count, uint32_t drawMode, int renderMode, uint32_t textureId)
{
	if (count == 0)
	{
		return;
	}

	glUseProgram(m_shaderProgram);

	glm::mat4 currentTransform = m_transformStack.back();
	glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, &currentTransform[0][0]);
	glUniform1i(m_renderModeLocation, renderMode);

	if (renderMode == 1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(m_textureLocation, 0);
	}

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * FLOATS_PER_VERTEX * sizeof(float)), data, GL_DYNAMIC_DRAW);

	glDrawArrays(drawMode, 0, static_cast<GLsizei>(count));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OpenglRenderer::CheckShaderCompilation(uint32_t shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		throw std::runtime_error("Ошибка компиляции шейдера");
	}
}

void OpenglRenderer::CheckProgramLinkage(uint32_t program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw std::runtime_error("Ошибка линковки шейдерной программы");
	}
}

uint32_t OpenglRenderer::CreateShaderProgram()
{
	auto vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec4 aColor;
		layout (location = 2) in vec2 aTexCoord;

		out vec4 vertexColor;
		out vec2 texCoord;

		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * vec4(aPos, 0.0, 1.0);
			vertexColor = aColor;
			texCoord = aTexCoord;
		}
	)";

	auto fragmentShaderSource = R"(
		#version 330 core
		in vec4 vertexColor;
		in vec2 texCoord;
		out vec4 FragColor;

		uniform sampler2D uTexture;
		uniform int uRenderMode;

		void main()
		{
			if (uRenderMode == 1)
			{
				float alpha = texture(uTexture, texCoord).r;
				FragColor = vec4(vertexColor.rgb, vertexColor.a * alpha);
			}
			else
			{
				FragColor = vertexColor;
			}
		}
	)";

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	CheckShaderCompilation(vertexShader);

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	CheckShaderCompilation(fragmentShader);

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	CheckProgramLinkage(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
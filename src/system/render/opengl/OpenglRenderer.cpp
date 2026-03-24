#include "OpenglRenderer.h"
#include "src/system/AppConfig.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <numbers>
#include <ranges>
#include <stdexcept>

#define STB_TRUETYPE_IMPLEMENTATION
#pragma warning(push, 0)
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
constexpr int FLOATS_PER_VERTEX_2D = 8;
constexpr int FLOATS_PER_VERTEX_LINE_3D = 7;
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

void AssertIsShaderCompiled(uint32_t shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glDeleteShader(shader);
		throw std::runtime_error("Ошибка компиляции встроенного шейдера рендерера");
	}
}

void AssertIsProgramLinked(uint32_t program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glDeleteProgram(program);
		throw std::runtime_error("Ошибка линковки встроенной шейдерной программы рендерера");
	}
}

uint32_t CompileInternalShader(uint32_t type, const std::string& source)
{
	uint32_t shader = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	AssertIsShaderCompiled(shader);

	return shader;
}

uint32_t CreateInternalShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
	uint32_t vertexShader = CompileInternalShader(GL_VERTEX_SHADER, vertexSource);
	uint32_t fragmentShader = CompileInternalShader(GL_FRAGMENT_SHADER, fragmentSource);

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	AssertIsProgramLinked(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void PushVertex2D(std::vector<float>& data, float x, float y, const Color& color, float u = 0.0f, float v = 0.0f)
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

void PushVertex3D(std::vector<float>& data, const Point3f& pos, const Color& color)
{
	data.push_back(pos.x);
	data.push_back(pos.y);
	data.push_back(pos.z);
	data.push_back(color.GetR());
	data.push_back(color.GetG());
	data.push_back(color.GetB());
	data.push_back(color.GetA());
}

void GenerateEllipseFill(std::vector<float>& data, const Point2f& center, const Point2f& radius, const Color& color)
{
	for (float i = 0.0f; i < CIRCLE_SEGMENTS; ++i)
	{
		float theta1 = 2.0f * std::numbers::pi_v<float> * i / CIRCLE_SEGMENTS;
		float theta2 = 2.0f * std::numbers::pi_v<float> * (i + 1.0f) / CIRCLE_SEGMENTS;

		PushVertex2D(data, center.x, center.y, color);
		PushVertex2D(data, center.x + radius.x * std::cos(theta1), center.y + radius.y * std::sin(theta1), color);
		PushVertex2D(data, center.x + radius.x * std::cos(theta2), center.y + radius.y * std::sin(theta2), color);
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

		PushVertex2D(data, x1Inner, y1Inner, color);
		PushVertex2D(data, x1Outer, y1Outer, color);
		PushVertex2D(data, x2Outer, y2Outer, color);

		PushVertex2D(data, x1Inner, y1Inner, color);
		PushVertex2D(data, x2Outer, y2Outer, color);
		PushVertex2D(data, x2Inner, y2Inner, color);
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

const std::string SHADER_2D_VERT = R"(
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

const std::string SHADER_2D_FRAG = R"(
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

const std::string SHADER_LINE_3D_VERT = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	out vec4 vertexColor;

	uniform mat4 u_Projection;
	uniform mat4 u_View;

	void main()
	{
		gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
		vertexColor = aColor;
	}
)";

const std::string SHADER_LINE_3D_FRAG = R"(
	#version 330 core
	in vec4 vertexColor;
	out vec4 FragColor;

	void main()
	{
		FragColor = vertexColor;
	}
)";
} // namespace

OpenglRenderer::OpenglRenderer()
	: m_shader2D(0)
	, m_vao2D(0)
	, m_vbo2D(0)
	, m_projectionLocation2D(0)
	, m_renderModeLocation2D(0)
	, m_textureLocation2D(0)
	, m_shaderLines3D(0)
	, m_vaoLines3D(0)
	, m_vboLines3D(0)
	, m_projectionLocationLines3D(0)
	, m_viewLocationLines3D(0)
	, m_nextFontId(1)
{
	AssertIsGladInitialized(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));

	m_shader2D = CreateInternalShaderProgram(SHADER_2D_VERT, SHADER_2D_FRAG);
	m_projectionLocation2D = glGetUniformLocation(m_shader2D, "projection");
	m_renderModeLocation2D = glGetUniformLocation(m_shader2D, "uRenderMode");
	m_textureLocation2D = glGetUniformLocation(m_shader2D, "uTexture");

	glGenVertexArrays(1, &m_vao2D);
	glGenBuffers(1, &m_vbo2D);
	glBindVertexArray(m_vao2D);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2D);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX_2D * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX_2D * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX_2D * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shaderLines3D = CreateInternalShaderProgram(SHADER_LINE_3D_VERT, SHADER_LINE_3D_FRAG);
	m_projectionLocationLines3D = glGetUniformLocation(m_shaderLines3D, "u_Projection");
	m_viewLocationLines3D = glGetUniformLocation(m_shaderLines3D, "u_View");

	glGenVertexArrays(1, &m_vaoLines3D);
	glGenBuffers(1, &m_vboLines3D);
	glBindVertexArray(m_vaoLines3D);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboLines3D);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX_LINE_3D * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX_LINE_3D * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

OpenglRenderer::~OpenglRenderer()
{
	for (const auto& font : m_fonts | std::views::values)
	{
		glDeleteTextures(1, &font->textureId);
	}

	glDeleteVertexArrays(1, &m_vao2D);
	glDeleteBuffers(1, &m_vbo2D);
	glDeleteProgram(m_shader2D);

	glDeleteVertexArrays(1, &m_vaoLines3D);
	glDeleteBuffers(1, &m_vboLines3D);
	glDeleteProgram(m_shaderLines3D);
}

void OpenglRenderer::SetClearColor(const Color& color)
{
	glClearColor(color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void OpenglRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderer::BeginFrame(const CameraData& camera)
{
	m_camera = camera;
	m_meshCommands.clear();
	m_lights.clear();
	m_lineCommands.clear();
}

void OpenglRenderer::SubmitLight(const LightData& light)
{
	m_lights.push_back(light);
}

void OpenglRenderer::SubmitMesh(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material, const Transform& transform)
{
	m_meshCommands.push_back({mesh, material, transform});
}

void OpenglRenderer::DrawLine3D(const Point3f& start, const Point3f& end, const Color& color, float thickness)
{
	m_lineCommands.push_back({start, end, color, thickness});
}

void OpenglRenderer::EndFrame()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	for (const auto& cmd : m_meshCommands)
	{
		cmd.material->Bind();
		auto shader = cmd.material->GetShader();

		shader->SetMat4("u_View", m_camera.viewMatrix);
		shader->SetMat4("u_Projection", m_camera.projectionMatrix);
		shader->SetMat4("u_Model", cmd.transform.GetMatrix());
		shader->SetFloat3("u_ViewPos", m_camera.position);

		int lightIndex = 0;
		for (const auto& light : m_lights)
		{
			std::string prefix = "u_Lights[" + std::to_string(lightIndex) + "].";
			shader->SetInt(prefix + "type", static_cast<int>(light.type));
			shader->SetColor(prefix + "color", light.color);
			shader->SetFloat3(prefix + "position", light.position);
			shader->SetFloat3(prefix + "direction", light.direction);
			shader->SetFloat(prefix + "intensity", light.intensity);
			lightIndex++;
		}

		shader->SetInt("u_LightCount", lightIndex);

		cmd.mesh->Bind();
		glDrawElements(GL_TRIANGLES, cmd.mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		cmd.mesh->Unbind();

		cmd.material->Unbind();
	}

	Flush3DLines();
}

void OpenglRenderer::Flush3DLines()
{
	if (m_lineCommands.empty())
	{
		return;
	}

	glUseProgram(m_shaderLines3D);
	glUniformMatrix4fv(m_projectionLocationLines3D, 1, GL_FALSE, glm::value_ptr(m_camera.projectionMatrix));
	glUniformMatrix4fv(m_viewLocationLines3D, 1, GL_FALSE, glm::value_ptr(m_camera.viewMatrix));

	glBindVertexArray(m_vaoLines3D);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboLines3D);

	for (const auto& cmd : m_lineCommands)
	{
		std::vector<float> data;
		PushVertex3D(data, cmd.start, cmd.color);
		PushVertex3D(data, cmd.end, cmd.color);

		glLineWidth(cmd.thickness);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glLineWidth(1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void OpenglRenderer::BeginUI()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenglRenderer::EndUI()
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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

void OpenglRenderer::DrawRect(const Point2f& position, const Point2f& size, const RenderStyle& style)
{
	std::vector<float> data;
	PushVertex2D(data, position.x, position.y, style.fillColor);
	PushVertex2D(data, position.x + size.x, position.y, style.fillColor);
	PushVertex2D(data, position.x, position.y + size.y, style.fillColor);
	PushVertex2D(data, position.x + size.x, position.y, style.fillColor);
	PushVertex2D(data, position.x + size.x, position.y + size.y, style.fillColor);
	PushVertex2D(data, position.x, position.y + size.y, style.fillColor);

	Render2DGeometry(data.data(), data.size() / FLOATS_PER_VERTEX_2D, GL_TRIANGLES);
}

void OpenglRenderer::DrawRoundedRect(const Point2f& position, const Point2f& size, float radius, const RenderStyle& style)
{
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
	float pi = std::numbers::pi_v<float>;

	GenerateArcPoints(points, {position.x + w - radius, position.y + radius}, radius, 1.5f * pi, 2.0f * pi);
	GenerateArcPoints(points, {position.x + w - radius, position.y + h - radius}, radius, 0.0f, 0.5f * pi);
	GenerateArcPoints(points, {position.x + radius, position.y + h - radius}, radius, 0.5f * pi, pi);
	GenerateArcPoints(points, {position.x + radius, position.y + radius}, radius, pi, 1.5f * pi);

	DrawPolygon(points, style);
}

void OpenglRenderer::DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style)
{
	std::vector<float> fillData;
	GenerateEllipseFill(fillData, center, radius, style.fillColor);
	Render2DGeometry(fillData.data(), fillData.size() / FLOATS_PER_VERTEX_2D, GL_TRIANGLES);

	if (style.outlineThickness > 0.0f)
	{
		std::vector<float> outlineData;
		GenerateEllipseOutline(outlineData, center, radius, style.outlineThickness, style.outlineColor);
		Render2DGeometry(outlineData.data(), outlineData.size() / FLOATS_PER_VERTEX_2D, GL_TRIANGLES);
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
		PushVertex2D(data, points[0].x, points[0].y, style.fillColor);
		PushVertex2D(data, points[i].x, points[i].y, style.fillColor);
		PushVertex2D(data, points[i + 1].x, points[i + 1].y, style.fillColor);
	}

	Render2DGeometry(data.data(), data.size() / FLOATS_PER_VERTEX_2D, GL_TRIANGLES);
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
		PushVertex2D(data, point.x, point.y, color);
	}

	Render2DGeometry(data.data(), data.size() / FLOATS_PER_VERTEX_2D, GL_LINE_STRIP);
	glLineWidth(1.0f);
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

		PushVertex2D(data, quad.x0, quad.y0, color, quad.s0, quad.t0);
		PushVertex2D(data, quad.x1, quad.y0, color, quad.s1, quad.t0);
		PushVertex2D(data, quad.x0, quad.y1, color, quad.s0, quad.t1);
		PushVertex2D(data, quad.x1, quad.y0, color, quad.s1, quad.t0);
		PushVertex2D(data, quad.x1, quad.y1, color, quad.s1, quad.t1);
		PushVertex2D(data, quad.x0, quad.y1, color, quad.s0, quad.t1);
	}

	Render2DGeometry(data.data(), data.size() / FLOATS_PER_VERTEX_2D, GL_TRIANGLES, 1, font->textureId);
}

void OpenglRenderer::Render2DGeometry(const float* data, size_t count, uint32_t drawMode, int renderMode, uint32_t textureId)
{
	if (count == 0)
	{
		return;
	}

	glUseProgram(m_shader2D);

	glm::mat4 ortho = glm::ortho(0.0f, AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(m_projectionLocation2D, 1, GL_FALSE, glm::value_ptr(ortho));
	glUniform1i(m_renderModeLocation2D, renderMode);

	if (renderMode == 1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(m_textureLocation2D, 0);
	}

	glBindVertexArray(m_vao2D);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2D);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * FLOATS_PER_VERTEX_2D * sizeof(float)), data, GL_DYNAMIC_DRAW);

	glDrawArrays(drawMode, 0, static_cast<GLsizei>(count));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
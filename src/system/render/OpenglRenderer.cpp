#include "OpenglRenderer.h"
#include "src/system/AppConfig.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <numbers>
#include <stdexcept>

namespace
{
constexpr float CIRCLE_SEGMENTS = 64.0f;
constexpr int FLOATS_PER_VERTEX = 6;

void AssertIsGladInitialized(int result)
{
	if (!result)
	{
		throw std::runtime_error("Не удалось инициализировать GLAD");
	}
}

void PushVertex(std::vector<float>& data, float x, float y, const Color& color)
{
	data.push_back(x);
	data.push_back(y);
	data.push_back(color.GetR());
	data.push_back(color.GetG());
	data.push_back(color.GetB());
	data.push_back(color.GetA());
}
} // namespace

OpenglRenderer::OpenglRenderer()
	: m_shaderProgram(0)
	, m_vao(0)
	, m_vbo(0)
	, m_projectionLocation(0)
{
	AssertIsGladInitialized(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);

	m_shaderProgram = CreateShaderProgram();
	m_projectionLocation = glGetUniformLocation(m_shaderProgram, "projection");

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	PushTransform();
}

OpenglRenderer::~OpenglRenderer()
{
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

	GLint windowHeight = AppConfig::WINDOW_HEIGHT;
	GLint scissorY = windowHeight - static_cast<GLint>(position.y + size.y);

	glScissor(
		static_cast<GLint>(position.x),
		scissorY,
		static_cast<GLsizei>(size.x),
		static_cast<GLsizei>(size.y));
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

void OpenglRenderer::DrawRoundedRect(const Point2f&, const Point2f&, float, const RenderStyle&)
{
	throw std::runtime_error("Отрисовка прямоугольника с закруглением пока не реализована");
}

void OpenglRenderer::DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style)
{
	std::vector<float> data;

	for (float i = 0; i < CIRCLE_SEGMENTS; ++i)
	{
		float theta1 = 2.0f * std::numbers::pi_v<float> * i / CIRCLE_SEGMENTS;
		float theta2 = 2.0f * std::numbers::pi_v<float> * (i + 1.0f) / CIRCLE_SEGMENTS;

		PushVertex(data, center.x, center.y, style.fillColor);
		PushVertex(data, center.x + radius.x * std::cos(theta1), center.y + radius.y * std::sin(theta1), style.fillColor);
		PushVertex(data, center.x + radius.x * std::cos(theta2), center.y + radius.y * std::sin(theta2), style.fillColor);
	}

	RenderGeometry(data.data(), data.size() / FLOATS_PER_VERTEX, GL_TRIANGLES);
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
	throw std::runtime_error("Отрисовка кривой Безье пока не реализована");
}

void OpenglRenderer::DrawTextData(const Point2f&, std::string_view, uint32_t, float, const Color&)
{
	throw std::runtime_error("Отрисовка текста не реализована. Требуется менеджер шрифтов.");
}

void OpenglRenderer::DrawTexture(const Point2f&, const Point2f&, const ITexture*)
{
	throw std::runtime_error("Отрисовка текстуры не реализована.");
}

void OpenglRenderer::DrawTexturePart(const Point2f&, const Point2f&, const Point2f&, const Point2f&, const ITexture*)
{
	throw std::runtime_error("Отрисовка части текстуры не реализована.");
}

void OpenglRenderer::RenderGeometry(const float* data, size_t count, uint32_t drawMode)
{
	if (count == 0)
	{
		return;
	}

	glUseProgram(m_shaderProgram);

	glm::mat4 currentTransform = m_transformStack.back();
	glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, &currentTransform[0][0]);

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

		out vec4 vertexColor;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * vec4(aPos, 0.0, 1.0);
			vertexColor = aColor;
		}
	)";

	auto fragmentShaderSource = R"(
		#version 330 core
		in vec4 vertexColor;
		out vec4 FragColor;

		void main()
		{
			FragColor = vertexColor;
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
#include "OpenglRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <numbers>
#include <stdexcept>
#include <vector>

namespace
{
constexpr int CIRCLE_SEGMENTS = 64;
constexpr int CORNER_SEGMENTS = 16;
constexpr int FLOATS_PER_VERTEX = 6;

void AssertIsGladInitialized(int result)
{
	if (!result)
	{
		throw std::runtime_error("Не удалось инициализировать GLAD");
	}
}

void AssertIsShaderCompiled(uint32_t shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		throw std::runtime_error("Ошибка компиляции шейдера");
	}
}

void AssertIsProgramLinked(uint32_t program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw std::runtime_error("Ошибка линковки шейдерной программы");
	}
}

uint32_t CreateShaderProgram()
{
	const char* vertexShaderSource = R"(
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

	const char* fragmentShaderSource = R"(
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
	AssertIsShaderCompiled(vertexShader);

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	AssertIsShaderCompiled(fragmentShader);

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	AssertIsProgramLinked(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void PushVertex(std::vector<float>& data, float x, float y, const Color& color)
{
	data.push_back(x);
	data.push_back(y);
	data.push_back(static_cast<float>(color.r) / 255.0f);
	data.push_back(static_cast<float>(color.g) / 255.0f);
	data.push_back(static_cast<float>(color.b) / 255.0f);
	data.push_back(static_cast<float>(color.a) / 255.0f);
}

void AppendCircleTriangles(std::vector<float>& data, const Point2f& center, float radius, const Color& color)
{
	for (int i = 0; i < CIRCLE_SEGMENTS; ++i)
	{
		float theta1 = 2.0f * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(CIRCLE_SEGMENTS);
		float theta2 = 2.0f * std::numbers::pi_v<float> * static_cast<float>(i + 1) / static_cast<float>(CIRCLE_SEGMENTS);

		PushVertex(data, center.x, center.y, color);
		PushVertex(data, center.x + radius * std::cos(theta1), center.y + radius * std::sin(theta1), color);
		PushVertex(data, center.x + radius * std::cos(theta2), center.y + radius * std::sin(theta2), color);
	}
}

void AppendRectangleTriangles(std::vector<float>& data, const Point2i& pos, const Point2i& size, const Color& color)
{
	float x = static_cast<float>(pos.x);
	float y = static_cast<float>(pos.y);
	float w = static_cast<float>(size.x);
	float h = static_cast<float>(size.y);

	PushVertex(data, x, y, color);
	PushVertex(data, x + w, y, color);
	PushVertex(data, x, y + h, color);

	PushVertex(data, x + w, y, color);
	PushVertex(data, x + w, y + h, color);
	PushVertex(data, x, y + h, color);
}

void AppendRoundedRectTriangles(std::vector<float>& data, const Point2i& pos, const Point2i& size, float radius, const Color& color)
{
	float x = static_cast<float>(pos.x);
	float y = static_cast<float>(pos.y);
	float w = static_cast<float>(size.x);
	float h = static_cast<float>(size.y);

	float safeRadius = std::min({radius, w / 2.0f, h / 2.0f});
	Point2f center{x + w / 2.0f, y + h / 2.0f};

	std::vector<Point2f> perimeter;
	Point2f corners[4] = {
		{x + w - safeRadius, y + h - safeRadius},
		{x + safeRadius, y + h - safeRadius},
		{x + safeRadius, y + safeRadius},
		{x + w - safeRadius, y + safeRadius}};

	for (int c = 0; c < 4; ++c)
	{
		float startAngle = static_cast<float>(c) * (std::numbers::pi_v<float> / 2.0f);
		for (int i = 0; i <= CORNER_SEGMENTS; ++i)
		{
			float angle = startAngle + static_cast<float>(i) / static_cast<float>(CORNER_SEGMENTS) * (std::numbers::pi_v<float> / 2.0f);
			perimeter.push_back({corners[c].x + safeRadius * std::cos(angle),
				corners[c].y + safeRadius * std::sin(angle)});
		}
	}

	for (size_t i = 0; i < perimeter.size(); ++i)
	{
		size_t next = (i + 1) % perimeter.size();
		PushVertex(data, center.x, center.y, color);
		PushVertex(data, perimeter[i].x, perimeter[i].y, color);
		PushVertex(data, perimeter[next].x, perimeter[next].y, color);
	}
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
}

OpenglRenderer::~OpenglRenderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteProgram(m_shaderProgram);
}

void OpenglRenderer::Clear(const Color& color)
{
	glClearColor(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f);

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

void OpenglRenderer::DrawCircle(const Point2f& center, float radius, float thickness, const Color& fillColor, const Color& outlineColor)
{
	std::vector<float> geometryData;

	if (thickness > 0.0f)
	{
		AppendCircleTriangles(geometryData, center, radius + thickness, outlineColor);
	}

	AppendCircleTriangles(geometryData, center, radius, fillColor);
	RenderGeometry(geometryData.data(), geometryData.size() / FLOATS_PER_VERTEX);
}

void OpenglRenderer::DrawRectangle(const Point2i& position, const Point2i& size, const Color& color)
{
	std::vector<float> geometryData;
	AppendRectangleTriangles(geometryData, position, size, color);
	RenderGeometry(geometryData.data(), geometryData.size() / FLOATS_PER_VERTEX);
}

void OpenglRenderer::DrawRoundedRectangle(const Point2i& position, const Point2i& size, float radius, const Color& color, const Color& outlineColor)
{
	std::vector<float> geometryData;

	Point2i innerPos{position.x + 2, position.y + 2};
	Point2i innerSize{size.x - 4, size.y - 4};

	AppendRoundedRectTriangles(geometryData, position, size, radius, outlineColor);
	AppendRoundedRectTriangles(geometryData, innerPos, innerSize, radius, color);

	RenderGeometry(geometryData.data(), geometryData.size() / FLOATS_PER_VERTEX);
}

void OpenglRenderer::DrawTextData(const Point2i&, const std::string&, float, const Color&)
{
	// throw std::runtime_error("Отрисовка текста не реализована. Требуется библиотека FreeType.");
}

void OpenglRenderer::RenderGeometry(const float* data, size_t count)
{
	if (count == 0)
	{
		return;
	}

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glm::mat4 projection = glm::ortho(
		0.0f,
		static_cast<float>(viewport[2]),
		static_cast<float>(viewport[3]),
		0.0f,
		-1.0f,
		1.0f);

	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * FLOATS_PER_VERTEX * sizeof(float)), data, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
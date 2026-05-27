#include "OpenGLRenderer.h"
#include <array>
#include <glad/glad.h>
#include <stdexcept>

namespace
{
constexpr auto VERTEX_SHADER_SOURCE = "static/shaders/fractal.vert";

constexpr uint8_t CANVAS_VERTICES_SIZE = 12u;
constexpr std::array<float, CANVAS_VERTICES_SIZE> CANVAS_VERTICES = {
	-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f};

std::string GetFragmentShaderPath(FractalType type)
{
	switch (type)
	{
	case FractalType::Mandelbrot: return "static/shaders/mandelbrot.frag";
	case FractalType::Julia: return "static/shaders/julia.frag";
	case FractalType::BurningShip: return "static/shaders/burningship.frag";
	default: throw std::invalid_argument("Неизвестный тип фрактала");
	}
}
} // namespace

OpenGLRenderer::OpenGLRenderer()
{
	InitGeometryCanvas();

	m_shaders[FractalType::Mandelbrot] = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, GetFragmentShaderPath(FractalType::Mandelbrot));
	m_shaders[FractalType::Julia] = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, GetFragmentShaderPath(FractalType::Julia));
	m_shaders[FractalType::BurningShip] = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, GetFragmentShaderPath(FractalType::BurningShip));
}

OpenGLRenderer::~OpenGLRenderer()
{
	glDeleteVertexArrays(1, &m_VertexArrayObject);
	glDeleteBuffers(1, &m_vertexBufferObject);
}

void OpenGLRenderer::SetViewport(uint32_t width, uint32_t height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
}

void OpenGLRenderer::SetClearColor(const Color& color)
{
	auto [r, g, b, a] = color.GetAsFloats();
	glClearColor(r, g, b, a);
}

void OpenGLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Display()
{
	glFlush();
}

void OpenGLRenderer::DrawProcedural(const ModelData& data)
{
	auto it = m_shaders.find(data.type);
	if (it == m_shaders.end()) return;

	const auto& shader = it->second;
	shader->Use();

	shader->SetFloat2("u_resolution", static_cast<float>(m_viewportWidth), static_cast<float>(m_viewportHeight));
	shader->SetFloat2("u_offset", data.offsetX, data.offsetY);
	shader->SetFloat("u_zoom", data.zoom);

	glBindVertexArray(m_VertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void OpenGLRenderer::InitGeometryCanvas()
{
	glGenVertexArrays(1, &m_VertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, CANVAS_VERTICES_SIZE * sizeof(float), CANVAS_VERTICES.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
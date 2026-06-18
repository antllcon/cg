#include "OpenGLRenderer.h"
#include <array>
#include <glad/glad.h>
#include <stdexcept>

namespace
{
constexpr auto VERTEX_SHADER_SOURCE = "static/shaders/fractal.vert";

constexpr size_t CANVAS_VERTICES_COUNT = 12u;
constexpr uint32_t CANVAS_VERTEX_COMPONENTS = 2u;
constexpr std::array<float, CANVAS_VERTICES_COUNT> CANVAS_VERTICES = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f};

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
	: m_canvas(CANVAS_VERTICES, CANVAS_VERTEX_COMPONENTS)
{
	m_shaders[FractalType::Mandelbrot] = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, GetFragmentShaderPath(FractalType::Mandelbrot));
	m_shaders[FractalType::Julia] = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, GetFragmentShaderPath(FractalType::Julia));
	m_shaders[FractalType::BurningShip] = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, GetFragmentShaderPath(FractalType::BurningShip));
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

	m_canvas.Draw();
}
#include "OpenGLRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>
#include <vector>

namespace
{
constexpr auto VERTEX_SHADER_SOURCE = "static/shaders/cannabis.vert";
constexpr auto FRAGMENT_SHADER_SOURCE = "static/shaders/cannabis.frag";

constexpr uint32_t LINE_VERTEX_COMPONENTS = 2u;
constexpr float LINE_ANGLE_STEP = std::numbers::pi_v<float> / 1000.0f;
constexpr float LINE_MAX_ANGLE = 2.0f * std::numbers::pi_v<float>;

glm::mat4 MakeAspectProjection(float aspect)
{
	if (aspect >= 1.0f)
	{
		return glm::ortho(-aspect, aspect, -1.0f, 1.0f);
	}

	return glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
}

std::vector<float> GenerateStraightLine()
{
	std::vector<float> vertices;

	for (float angle = 0.0f; angle <= LINE_MAX_ANGLE; angle += LINE_ANGLE_STEP)
	{
		vertices.push_back(angle);
		vertices.push_back(0.0f);
	}

	return vertices;
}
} // namespace

OpenGLRenderer::OpenGLRenderer()
	: m_line(GenerateStraightLine(), LINE_VERTEX_COMPONENTS, PrimitiveType::LineStrip)
	, m_shader(std::make_unique<Shader>(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE))
{
}

void OpenGLRenderer::SetViewport(uint32_t width, uint32_t height)
{
	if (width == 0u || height == 0u)
	{
		return;
	}

	glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
	m_projection = MakeAspectProjection(static_cast<float>(width) / static_cast<float>(height));
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

void OpenGLRenderer::DrawProcedural(const ModelData&)
{
	m_shader->Use();
	m_shader->SetMatrix4("projection", glm::value_ptr(m_projection));
	m_line.Draw();
}
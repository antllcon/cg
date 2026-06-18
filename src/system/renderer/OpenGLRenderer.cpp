#include "OpenGLRenderer.h"
#include <cmath>
#include <glad/glad.h>
#include <numbers>
#include <vector>

namespace
{
constexpr auto VERTEX_SHADER_SOURCE = "static/shaders/cannabis.vert";
constexpr auto FRAGMENT_SHADER_SOURCE = "static/shaders/cannabis.frag";

constexpr uint32_t CURVE_VERTEX_COMPONENTS = 2u;
constexpr float CURVE_ANGLE_STEP = std::numbers::pi_v<float> / 1000.0f;
constexpr float CURVE_MAX_ANGLE = 2.0f * std::numbers::pi_v<float>;

float CannabisRadius(float angle)
{
	return (1.0f + std::sin(angle))
		* (1.0f + 0.9f * std::cos(8.0f * angle))
		* (1.0f + 0.1f * std::cos(24.0f * angle))
		* (0.5f + 0.05f * std::cos(140.0f * angle));
}

std::vector<float> GenerateCannabisCurve()
{
	std::vector<float> vertices;

	for (float angle = 0.0f; angle <= CURVE_MAX_ANGLE; angle += CURVE_ANGLE_STEP)
	{
		float radius = CannabisRadius(angle);
		vertices.push_back(radius * std::cos(angle));
		vertices.push_back(radius * std::sin(angle));
	}

	return vertices;
}
} // namespace

OpenGLRenderer::OpenGLRenderer()
	: m_curve(GenerateCannabisCurve(), CURVE_VERTEX_COMPONENTS, PrimitiveType::LineStrip)
	, m_shader(std::make_unique<Shader>(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE))
{
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
	m_curve.Draw();
}
#include "OpenGLRenderer.h"
#include <array>
#include <glad/glad.h>
#include <stdexcept>

namespace
{
constexpr auto VERTEX_SHADER_SOURCE = "static/shaders/image.vert";
constexpr auto FRAGMENT_SHADER_SOURCE = "static/shaders/image.frag";

constexpr float S_LEFT = -1.0f;
constexpr float S_RIGHT = 1.0f;
constexpr float S_BOTTOM = -1.0f;
constexpr float S_TOP = 1.0f;

constexpr float T_LEFT = 0.0f;
constexpr float T_RIGHT = 1.0f;
constexpr float T_BOTTOM = 0.0f;
constexpr float T_TOP = 1.0f;

constexpr uint8_t CANVAS_VERTICES_SIZE = 24u;

constexpr std::array<float, CANVAS_VERTICES_SIZE> CANVAS_VERTICES = {
	S_LEFT, S_BOTTOM, T_LEFT, T_BOTTOM, S_RIGHT, S_BOTTOM, T_RIGHT, T_BOTTOM, S_LEFT, S_TOP, T_LEFT, T_TOP, S_RIGHT, S_TOP, T_RIGHT, T_TOP, S_LEFT, S_TOP, T_LEFT, T_TOP, S_RIGHT, S_BOTTOM, T_RIGHT, T_BOTTOM};

void AssertIsImageValid(const Image* image)
{
	if (!image)
	{
		throw std::runtime_error("Передан пустой указатель на изображение");
	}
}

std::pair<float, float> CalculateScale(float windowWidth, float windowHeight, float imageWidth, float imageHeight)
{
	if (windowHeight == 0.0f || imageHeight == 0.0f)
	{
		return {1.0f, 1.0f};
	}

	float windowAspect = windowWidth / windowHeight;
	float imageAspect = imageWidth / imageHeight;

	if (windowAspect > imageAspect)
	{
		return {imageAspect / windowAspect, 1.0f};
	}

	return {1.0f, windowAspect / imageAspect};
}
} // namespace

OpenGLRenderer::OpenGLRenderer()
{
	InitGeometryCanvas();
	m_shader = std::make_unique<Shader>(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	glGenTextures(1, &m_texture);
}

OpenGLRenderer::~OpenGLRenderer()
{
	glDeleteVertexArrays(1, &m_VertexArrayObject);
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteTextures(1, &m_texture);
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

void OpenGLRenderer::DrawImage(const std::shared_ptr<Image>& image, const FilterSettings& settings)
{
	AssertIsImageValid(image.get());

	UpdateTextureIfNeeded(image);
	BindResourcesAndDraw(settings);
}

void OpenGLRenderer::InitGeometryCanvas()
{
	glGenVertexArrays(1, &m_VertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, CANVAS_VERTICES_SIZE * sizeof(float), CANVAS_VERTICES.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void OpenGLRenderer::UpdateTextureIfNeeded(const std::shared_ptr<Image>& image)
{
	if (m_loadedImage == image.get())
	{
		return;
	}

	m_loadedImage = image.get();

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	uint32_t format = image->IsRGB() ? GL_RGB : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, image->GetWidth(), image->GetHeight(), 0, format, GL_UNSIGNED_BYTE, image->GetData());

	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRenderer::BindResourcesAndDraw(const FilterSettings& settings) const
{
	m_shader->Use();
	m_shader->SetInt("imageTexture", 0);

	int useMedian = 0;
	int medianRadius = 0;

	if (const auto* medianSettings = std::get_if<MedianFilterSettings>(&settings))
	{
		useMedian = 1;
		medianRadius = medianSettings->radius;
	}

	m_shader->SetInt("useMedian", useMedian);
	m_shader->SetInt("medianRadius", medianRadius);

	auto [scaleX, scaleY] = CalculateScale(
		static_cast<float>(m_viewportWidth),
		static_cast<float>(m_viewportHeight),
		static_cast<float>(m_loadedImage->GetWidth()),
		static_cast<float>(m_loadedImage->GetHeight()));

	m_shader->SetFloat2("scale", scaleX, scaleY);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindVertexArray(m_VertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
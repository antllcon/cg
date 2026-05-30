#include "OpenGLRenderer.h"
#include "src/utils/PrimeVariant.h"
#include <algorithm> // ðöð╗ÐÅ std::clamp
#include <glad/glad.h>

namespace
{
constexpr auto VERTEX_SHADER_FILE = "static/shaders/image.vert";
constexpr auto FRAGMENT_SHADER_FILE = "static/shaders/image.frag";

std::pair<float, float> CalculateScale(float windowWidth, float windowHeight, float imageWidth, float imageHeight)
{
	if (windowHeight == 0.0f || imageHeight == 0.0f)
	{
		return {1.0f, 1.0f};
	}

	float windowRatio = windowWidth / windowHeight;
	float imageRatio = imageWidth / imageHeight;

	if (windowRatio > imageRatio)
	{
		return {imageRatio / windowRatio, 1.0f};
	}

	return {1.0f, windowRatio / imageRatio};
}
} // namespace

OpenGLRenderer::OpenGLRenderer()
{
	m_canvasMesh = std::make_unique<CanvasMesh>();
	m_shader = std::make_unique<Shader>(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	m_texture = std::make_unique<Texture>();
}

void OpenGLRenderer::SetViewport(uint32_t width, uint32_t height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));

	RecalculateScale();
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

void OpenGLRenderer::DrawImage(const Image& image, const FilterSettings& settings)
{
	UpdateTexture(image);
	BindResourcesAndDraw(settings);
}

void OpenGLRenderer::UpdateTexture(const Image& image)
{
	const Image* rawImage = &image;
	if (m_loadedImage == rawImage) return;
	m_loadedImage = rawImage;

	m_texelSizeX = 1.0f / static_cast<float>(image.GetWidth());
	m_texelSizeY = 1.0f / static_cast<float>(image.GetHeight());

	uint32_t format = image.IsRGB() ? GL_RGB : GL_RGBA;
	m_texture->UpdateData(image.GetWidth(), image.GetHeight(), format, image.GetData());

	RecalculateScale();
}

void OpenGLRenderer::BindResourcesAndDraw(const FilterSettings& settings) const
{
	m_shader->Use();
	m_shader->SetInt("imageTexture", 0);
	m_shader->SetFloat2("scale", m_scaleX, m_scaleY);
	m_shader->SetFloat2("texelSize", m_texelSizeX, m_texelSizeY);

	std::visit(overloaded{
				   [this](const NoFilterSettings&) {
					   m_shader->SetInt("useMedian", 0);
				   },
				   [this](const MedianFilterSettings& median) {
					   m_shader->SetInt("useMedian", 1);

					   int safeRadius = std::clamp(static_cast<int>(median.radius), 0, 7);
					   m_shader->SetInt("medianRadius", safeRadius);

					   int windowArea = (2 * safeRadius + 1) * (2 * safeRadius + 1);
					   int medianIndex = windowArea / 2;

					   m_shader->SetInt("windowArea", windowArea);
					   m_shader->SetInt("medianIndex", medianIndex);
				   }},
		settings);

	m_texture->Bind(0);
	m_canvasMesh->Bind();
	m_canvasMesh->Draw();
	m_canvasMesh->Unbind();
	m_texture->Unbind();
}

void OpenGLRenderer::RecalculateScale()
{
	if (!m_loadedImage)
	{
		return;
	}

	auto [x, y] = CalculateScale(
		static_cast<float>(m_viewportWidth),
		static_cast<float>(m_viewportHeight),
		static_cast<float>(m_loadedImage->GetWidth()),
		static_cast<float>(m_loadedImage->GetHeight()));

	m_scaleX = x;
	m_scaleY = y;
}
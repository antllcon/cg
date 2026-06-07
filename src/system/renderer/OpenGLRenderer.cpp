#include "OpenGLRenderer.h"
#include <cmath>
#include <glad/glad.h>
#include <vector>

namespace
{
constexpr auto VERTEX_SHADER_FILE = "static/shaders/image.vert";
constexpr auto FRAGMENT_SHADER_FILE = "static/shaders/image.frag";
constexpr auto GAUSSIAN_FRAGMENT_FILE = "static/shaders/gaussian.frag";
constexpr int MAX_KERNEL_RADIUS = 20;

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

std::vector<float> ComputeGaussianWeights(int radius)
{
	float sigma = std::max(1.0f, float(radius) / 3.0f);
	std::vector<float> weights(radius + 1);
	float sum = 0.0f;
	for (int i = 0; i <= radius; i++)
	{
		float w = std::exp(-0.5f * float(i * i) / (sigma * sigma));
		weights[i] = w;
		sum += (i == 0) ? w : 2.0f * w;
	}
	for (auto& w : weights)
	{
		w /= sum;
	}
	return weights;
}

void BindRawTexture(uint32_t textureId, uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
} // namespace

OpenGLRenderer::OpenGLRenderer()
{
	m_canvasMesh = std::make_unique<CanvasMesh>();
	m_shader = std::make_unique<Shader>(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	m_gaussianShader = std::make_unique<Shader>(VERTEX_SHADER_FILE, GAUSSIAN_FRAGMENT_FILE);
	m_texture = std::make_unique<Texture>();
	m_pingPongFBO[0] = std::make_unique<Framebuffer>(1, 1);
	m_pingPongFBO[1] = std::make_unique<Framebuffer>(1, 1);
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

	m_imageWidth = image.GetWidth();
	m_imageHeight = image.GetHeight();
	m_texelSizeX = 1.0f / static_cast<float>(m_imageWidth);
	m_texelSizeY = 1.0f / static_cast<float>(m_imageHeight);

	uint32_t format = image.IsRGB() ? GL_RGB : GL_RGBA;
	m_texture->UpdateData(m_imageWidth, m_imageHeight, format, image.GetData());

	ResizePingPongBuffers(m_imageWidth, m_imageHeight);
	RecalculateScale();
}

void OpenGLRenderer::BindResourcesAndDraw(const FilterSettings& settings) const
{
	if (const auto* gaussian = std::get_if<GaussianBlurSettings>(&settings))
	{
		ApplyGaussianBlur(*gaussian);
		return;
	}

	m_shader->Use();
	m_shader->SetInt("imageTexture", 0);
	m_shader->SetFloat2("scale", m_scaleX, m_scaleY);
	m_shader->SetFloat2("texelSize", m_texelSizeX, m_texelSizeY);
	m_shader->SetInt("flipY", 1);
	m_shader->SetInt("useMedian", 0);

	if (const auto* median = std::get_if<MedianFilterSettings>(&settings))
	{
		m_shader->SetInt("useMedian", 1);
		m_shader->SetInt("medianRadius", median->radius);
		int area = (2 * median->radius + 1) * (2 * median->radius + 1);
		m_shader->SetInt("medianArea", area);
		m_shader->SetInt("medianIndex", area / 2);
	}

	m_texture->Bind(0);
	m_canvasMesh->Bind();
	m_canvasMesh->Draw();
	m_canvasMesh->Unbind();
	m_texture->Unbind();
}

void OpenGLRenderer::ApplyGaussianBlur(const GaussianBlurSettings& settings) const
{
	int radius = settings.radius;
	int passes = (radius + MAX_KERNEL_RADIUS - 1) / MAX_KERNEL_RADIUS;
	int radiusPerPass = std::max(1, radius / passes);

	auto weights = ComputeGaussianWeights(radiusPerPass);

	m_gaussianShader->Use();
	m_gaussianShader->SetInt("imageTexture", 0);
	m_gaussianShader->SetInt("kernelRadius", radiusPerPass);
	m_gaussianShader->SetFloat2("scale", 1.0f, 1.0f);
	m_gaussianShader->SetFloat1v("weights", weights.data(), static_cast<int>(weights.size()));

	glViewport(0, 0, static_cast<int>(m_imageWidth), static_cast<int>(m_imageHeight));
	m_canvasMesh->Bind();

	for (int pass = 0; pass < passes; pass++)
	{
		m_pingPongFBO[0]->Bind();
		m_gaussianShader->SetFloat2("direction", m_texelSizeX, 0.0f);
		m_gaussianShader->SetInt("flipY", pass == 0 ? 1 : 0);

		if (pass == 0)
		{
			m_texture->Bind(0);
		}
		else
		{
			BindRawTexture(m_pingPongFBO[1]->GetTextureId(), 0);
		}
		m_canvasMesh->Draw();

		m_pingPongFBO[1]->Bind();
		m_gaussianShader->SetFloat2("direction", 0.0f, m_texelSizeY);
		m_gaussianShader->SetInt("flipY", 0);
		BindRawTexture(m_pingPongFBO[0]->GetTextureId(), 0);
		m_canvasMesh->Draw();
	}

	m_canvasMesh->Unbind();
	Framebuffer::Unbind();
	glViewport(0, 0, static_cast<int>(m_viewportWidth), static_cast<int>(m_viewportHeight));

	m_shader->Use();
	m_shader->SetInt("imageTexture", 0);
	m_shader->SetFloat2("scale", m_scaleX, m_scaleY);
	m_shader->SetFloat2("texelSize", m_texelSizeX, m_texelSizeY);
	m_shader->SetInt("flipY", 0);
	m_shader->SetInt("useMedian", 0);

	BindRawTexture(m_pingPongFBO[1]->GetTextureId(), 0);
	m_canvasMesh->Bind();
	m_canvasMesh->Draw();
	m_canvasMesh->Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRenderer::ResizePingPongBuffers(uint32_t width, uint32_t height)
{
	m_pingPongFBO[0]->Resize(width, height);
	m_pingPongFBO[1]->Resize(width, height);
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

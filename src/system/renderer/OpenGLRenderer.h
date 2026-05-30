#pragma once

#include "IRenderer.h"
#include "mesh/CanvasMesh.h"
#include "shader/Shader.h"
#include "src/utils/image/Image.h"
#include "texture/Texture.h"

#include <memory>

class OpenGLRenderer final : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer() override = default;

	void SetViewport(uint32_t width, uint32_t height) override;
	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;
	void DrawImage(const Image& image, const FilterSettings& settings) override;

private:
	void UpdateTexture(const Image& image);
	void BindResourcesAndDraw(const FilterSettings& settings) const;
	void RecalculateScale();

private:
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;

	float m_scaleX = 1.0f;
	float m_scaleY = 1.0f;

	float m_texelSizeX = 1.0f;
	float m_texelSizeY = 1.0f;

	const Image* m_loadedImage = nullptr;

	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<CanvasMesh> m_canvasMesh;
};
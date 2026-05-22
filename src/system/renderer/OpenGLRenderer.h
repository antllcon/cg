#pragma once

#include "IRenderer.h"
#include "shader/Shader.h"
#include "src/utils/image/Image.h"
#include <memory>

class OpenGLRenderer final : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer() override;

	void SetViewport(uint32_t width, uint32_t height) override;
	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;
	void DrawImage(const std::shared_ptr<Image>& image, const FilterSettings& settings) override;

private:
	void InitializeGeometry();
	void UpdateTextureIfNeeded(const std::shared_ptr<Image>& image);
	void BindResourcesAndDraw(const FilterSettings& settings) const;

private:
	uint32_t m_vao = 0;
	uint32_t m_vbo = 0;
	uint32_t m_texture = 0;
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;
	const Image* m_loadedImage = nullptr;
	std::unique_ptr<Shader> m_shader;
};
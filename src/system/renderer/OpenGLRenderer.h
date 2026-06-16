#pragma once

#include "IRenderer.h"
#include "shader/Shader.h"

#include <map>

class OpenGLRenderer final : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer() override;

	void SetViewport(uint32_t width, uint32_t height) override;
	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;
	void RenderFrame(const RenderData& data) override;

private:
	void InitShaders();
	void InitGeometry();

private:
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;
	Color m_clearColor;

	std::map<std::string, std::unique_ptr<Shader>> m_shaders;
};
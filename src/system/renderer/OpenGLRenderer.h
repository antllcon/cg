#pragma once

#include "IRenderer.h"
#include "mesh/Mesh.h"
#include "shader/Shader.h"
#include <map>
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
	void DrawProcedural(const ModelData& data) override;

private:
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;

	Mesh m_canvas;
	std::map<FractalType, std::unique_ptr<Shader>> m_shaders;
};
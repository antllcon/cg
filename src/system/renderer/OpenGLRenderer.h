#pragma once

#include "IRenderer.h"
#include "shader/Shader.h"
#include <map>
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
	void DrawProcedural(const ModelData& data) override;

private:
	void InitGeometryCanvas();

private:
	uint32_t m_VertexArrayObject = 0;
	uint32_t m_vertexBufferObject = 0;
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;

	std::map<FractalType, std::unique_ptr<Shader>> m_shaders;
};
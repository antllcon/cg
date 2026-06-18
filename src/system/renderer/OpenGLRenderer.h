#pragma once

#include "IRenderer.h"
#include "mesh/Mesh.h"
#include "shader/Shader.h"
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
	Mesh m_line;
	std::unique_ptr<Shader> m_shader;
	float m_aspect = 1.0f;
};
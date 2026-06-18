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

	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;
	void DrawProcedural(const ModelData& data) override;

private:
	Mesh m_curve;
	std::unique_ptr<Shader> m_shader;
};
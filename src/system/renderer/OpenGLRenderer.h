#pragma once

#include "IRenderer.h"
#include "framebuffer/Framebuffer.h"
#include "mash/Mesh.h"
#include "shader/Shader.h"

#include <array>
#include <map>
#include <memory>
#include <string>

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
	void InitFramebuffers();

	void RenderGeometryPass(const RenderData& data);
	void DownscaleGlowPass();
	void ApplyBlurPass();
	void ApplyLightMotionBlurPass();
	void RenderCompositePass();
	void DrawObject(const SceneObject& object, const std::unique_ptr<Shader>& shader);

private:
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;
	Color m_clearColor;

	std::unique_ptr<Mesh> m_canvasMesh;
	std::unique_ptr<Mesh> m_cylinderMesh;
	std::unique_ptr<Mesh> m_torusMesh;

	std::unique_ptr<Framebuffer> m_mainFbo;
	std::unique_ptr<Framebuffer> m_glowFbo;
	std::array<std::unique_ptr<Framebuffer>, 2> m_pingPongFbos;

	std::map<std::string, std::unique_ptr<Shader>> m_shaders;
};
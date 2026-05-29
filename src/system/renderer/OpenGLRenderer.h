#pragma once

#include "IRenderer.h"
#include "shader/Shader.h"
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
	void RenderFrame(const ModelData& data) override;

private:
	void InitShaders();
	void InitGeometry();
	void InitFramebuffers();
	void CleanupResources();

	void RenderMainPass(const ModelData& data);
	void RenderGlowMaskPass(const ModelData& data);
	void ApplyBlurPass();
	void RenderCompositePass();

	void DrawObject(const SceneObject& object, const std::unique_ptr<Shader>& shader);

private:
	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;
	Color m_clearColor;

	uint32_t m_canvasVao = 0;
	uint32_t m_canvasVbo = 0;

	uint32_t m_cylinderVao = 0;
	uint32_t m_cylinderVbo = 0;
	uint32_t m_cylinderVertexCount = 0;

	uint32_t m_torusVao = 0;
	uint32_t m_torusVbo = 0;
	uint32_t m_torusVertexCount = 0;

	uint32_t m_mainFbo = 0;
	uint32_t m_mainColorTex = 0;
	uint32_t m_mainDepthRbo = 0;

	uint32_t m_glowFbo = 0;
	uint32_t m_glowColorTex = 0;
	uint32_t m_glowDepthRbo = 0;

	uint32_t m_pingPongFbo[2] = {0, 0};
	uint32_t m_pingPongTex[2] = {0, 0};

	std::map<std::string, std::unique_ptr<Shader>> m_shaders;
};
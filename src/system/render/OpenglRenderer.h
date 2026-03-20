#pragma once
#include "src/core/interfaces/IRenderer.h"
#include <cstdint>
#include <libs/glm/glm.hpp>

class OpenglRenderer final : public IRenderer
{
public:
	OpenglRenderer();
	~OpenglRenderer() override;

	void Clear(const Color& color) override;
	void Display() override;

	void PushTransform() override;
	void PopTransform() override;
	void SetClipRect(const Point2f& position, const Point2f& size) override;

	void DrawPoint(const Point2f& position, const Color& color) override;
	void DrawLine(const Point2f& start, const Point2f& end, const Color& color, float thickness = 1.0f) override;

	void DrawRect(const Point2f& position, const Point2f& size, const RenderStyle& style) override;
	void DrawRoundedRect(const Point2f& position, const Point2f& size, float radius, const RenderStyle& style) override;

	void DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style) override;

	void DrawPolygon(const std::vector<Point2f>& points, const RenderStyle& style) override;
	void DrawPolyline(const std::vector<Point2f>& points, const Color& color, float thickness = 1.0f) override;

	void DrawBezier(const Point2f& p0, const Point2f& p1, const Point2f& p2, const Point2f& p3, const Color& color, float thickness = 1.0f) override;

	void DrawTextData(const Point2f& position, std::string_view text, uint32_t fontId, float fontSize, const Color& color) override;

	void DrawTexture(const Point2f& position, const Point2f& size, const ITexture* texture) override;
	void DrawTexturePart(const Point2f& position, const Point2f& size, const Point2f& uvStart, const Point2f& uvEnd, const ITexture* texture) override;

private:
	void RenderGeometry(const float* data, size_t count, uint32_t drawMode);
	static void CheckShaderCompilation(uint32_t shader);
	static void CheckProgramLinkage(uint32_t program);
	static uint32_t CreateShaderProgram();

private:
	uint32_t m_shaderProgram;
	uint32_t m_vao;
	uint32_t m_vbo;
	int32_t m_projectionLocation;

	std::vector<glm::mat4> m_transformStack;
};
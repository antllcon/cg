#pragma once
#include "src/core/interfaces/IRenderer.h"
#include <cstdint>

class OpenglRenderer final : public IRenderer
{
public:
	OpenglRenderer();
	~OpenglRenderer() override;

	void Clear(const Color& color) override;
	void Display() override;

	void DrawCircle(const Point2f& center, float radius, float thickness, const Color& fillColor, const Color& outlineColor) override;
	void DrawRectangle(const Point2i& position, const Point2i& size, const Color& color) override;
	void DrawRoundedRectangle(const Point2i& position, const Point2i& size, float radius, const Color& color, const Color& outlineColor) override;
	void DrawTextData(const Point2i& position, const std::string& text, float fontSize, const Color& color) override;

private:
	void RenderGeometry(const float* data, size_t count);

	uint32_t m_shaderProgram;
	uint32_t m_vao;
	uint32_t m_vbo;
	int32_t m_projectionLocation;
};
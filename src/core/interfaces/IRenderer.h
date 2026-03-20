#pragma once
#include "src/core/types/Point.h"
#include "src/core/types/color/Color.h"
#include <string>
#include <vector>

struct RenderStyle
{
	Color fillColor;
	Color outlineColor;
	float outlineThickness{0.0f};
};

class ITexture;

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void Clear(const Color& color) = 0;
	virtual void Display() = 0;

	virtual void PushTransform() = 0;
	virtual void PopTransform() = 0;
	virtual void SetClipRect(const Point2f& position, const Point2f& size) = 0;

	virtual void DrawPoint(const Point2f& position, const Color& color) = 0;
	virtual void DrawLine(const Point2f& start, const Point2f& end, const Color& color, float thickness = 1.0f) = 0;

	virtual void DrawRect(const Point2f& position, const Point2f& size, const RenderStyle& style) = 0;
	virtual void DrawRoundedRect(const Point2f& position, const Point2f& size, float radius, const RenderStyle& style) = 0;

	virtual void DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style) = 0;

	virtual void DrawPolygon(const std::vector<Point2f>& points, const RenderStyle& style) = 0;
	virtual void DrawPolyline(const std::vector<Point2f>& points, const Color& color, float thickness = 1.0f) = 0;

	virtual void DrawBezier(const Point2f& p0, const Point2f& p1, const Point2f& p2, const Point2f& p3, const Color& color, float thickness = 1.0f) = 0;

	virtual void DrawTextData(const Point2f& position, std::string_view text, uint32_t fontId, float fontSize, const Color& color) = 0;

	virtual void DrawTexture(const Point2f& position, const Point2f& size, const ITexture* texture) = 0;
	virtual void DrawTexturePart(const Point2f& position, const Point2f& size, const Point2f& uvStart, const Point2f& uvEnd, const ITexture* texture) = 0;
};
#pragma once
#include "../types/Color.h"
#include "src/core/types/Point.h"
#include <string>

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void Clear(const Color& color) = 0;
	virtual void Display() = 0;
	virtual void DrawCircle(const Point2f& center, float radius, float thickness, const Color& fillColor, const Color& outlineColor) = 0;
	virtual void DrawRectangle(const Point2i& position, const Point2i& size, const Color& color) = 0;
	virtual void DrawRoundedRectangle(const Point2i& position, const Point2i& size, float radius, const Color& color, const Color& outlineColor) = 0;
	virtual void DrawTextData(const Point2i& position, const std::string& text, float fontSize, const Color& color) = 0;
};
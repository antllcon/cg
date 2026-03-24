#pragma once
#include "src/core/types/Point.h"
#include "src/core/types/color/Color.h"
#include "src/core/types/math/Transform.h"
#include "src/core/types/scene/CameraData.h"
#include "src/core/types/scene/LightData.h"
#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

class IMesh;
class IMaterial;

struct RenderStyle
{
	Color fillColor;
	Color outlineColor;
	float outlineThickness{0.0f};
};

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void SetClearColor(const Color& color) = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;

	virtual void BeginFrame(const CameraData& camera) = 0;
	virtual void EndFrame() = 0;

	virtual void SubmitLight(const LightData& light) = 0;
	virtual void SubmitMesh(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material, const Transform& transform) = 0;
	virtual void DrawLine3D(const Point3f& start, const Point3f& end, const Color& color, float thickness = 1.0f) = 0;

	virtual void BeginUI() = 0;
	virtual void EndUI() = 0;

	virtual void SetClipRect(const Point2f& position, const Point2f& size) = 0;
	virtual uint32_t LoadFont(const std::filesystem::path& path, float fontSize) = 0;

	virtual void DrawRect(const Point2f& position, const Point2f& size, const RenderStyle& style) = 0;
	virtual void DrawRoundedRect(const Point2f& position, const Point2f& size, float radius, const RenderStyle& style) = 0;
	virtual void DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style) = 0;
	virtual void DrawPolygon(const std::vector<Point2f>& points, const RenderStyle& style) = 0;
	virtual void DrawPolyline(const std::vector<Point2f>& points, const Color& color, float thickness = 1.0f) = 0;
	virtual void DrawTextData(const Point2f& position, std::string_view text, uint32_t fontId, const Color& color) = 0;
};
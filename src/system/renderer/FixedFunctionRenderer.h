#pragma once

#include "IRenderer.h"
#include "RenderableShape.h"
#include "geometry/MobiusStrip.h"
#include <map>
#include <vector>

class FixedFunctionRenderer final : public IRenderer
{
public:
	FixedFunctionRenderer();
	~FixedFunctionRenderer() override = default;

	void SetViewport(uint32_t width, uint32_t height) override;
	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;
	void RenderFrame(const RenderData& data) override;

private:
	void ApplyCamera(const CameraState& camera) const;
	void ApplyLight(const Light& light) const;
	void DrawObject(const SceneObject& object) const;

	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;
	Color m_clearColor;

	std::map<RenderableShape, std::vector<Geometry::Vertex>> m_shapes;
};
#pragma once

#include "IRenderer.h"
#include "RenderableShape.h"
#include "SkySphere.h"
#include "geometry/Cube.h"
#include "gl/GlMultiTexture.h"
#include "texture/TextureLibrary.h"
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
	void DrawObject(const SceneObject& object) const;
	void DrawGeometry(const std::vector<Geometry::Vertex>& vertices, float uvScaleU, float uvScaleV, bool useShadow) const;

	void BindBaseTexture(TextureKey texture) const;
	void BindShadowTexture() const;
	void UnbindShadowTexture() const;

	uint32_t m_viewportWidth = 1;
	uint32_t m_viewportHeight = 1;
	Color m_clearColor;

	std::map<RenderableShape, std::vector<Geometry::Vertex>> m_shapes;
	TextureLibrary m_textures;
	GlMultiTexture m_multiTexture;
	SkySphere m_sky;
};

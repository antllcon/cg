#pragma once
#include "src/core/interfaces/IMaterial.h"
#include "src/core/interfaces/IMesh.h"
#include "src/core/interfaces/IRenderer.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

struct FontData;

struct MeshCommand
{
	std::shared_ptr<IMesh> mesh;
	std::shared_ptr<IMaterial> material;
	Transform transform;
};

struct LineCommand
{
	Point3f start;
	Point3f end;
	Color color;
	float thickness;
};

class OpenglRenderer final : public IRenderer
{
public:
	OpenglRenderer();
	~OpenglRenderer() override;

	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;

	void BeginFrame(const CameraData& camera) override;
	void EndFrame() override;

	void SubmitLight(const LightData& light) override;
	void SubmitMesh(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material, const Transform& transform) override;
	void DrawLine3D(const Point3f& start, const Point3f& end, const Color& color, float thickness = 1.0f) override;

	void BeginUI() override;
	void EndUI() override;

	void SetClipRect(const Point2f& position, const Point2f& size) override;
	uint32_t LoadFont(const std::filesystem::path& path, float fontSize) override;

	void DrawRect(const Point2f& position, const Point2f& size, const RenderStyle& style) override;
	void DrawRoundedRect(const Point2f& position, const Point2f& size, float radius, const RenderStyle& style) override;
	void DrawEllipse(const Point2f& center, const Point2f& radius, const RenderStyle& style) override;
	void DrawPolygon(const std::vector<Point2f>& points, const RenderStyle& style) override;
	void DrawPolyline(const std::vector<Point2f>& points, const Color& color, float thickness = 1.0f) override;
	void DrawTextData(const Point2f& position, std::string_view text, uint32_t fontId, const Color& color) override;

private:
	void Render2DGeometry(const float* data, size_t count, uint32_t drawMode, int renderMode = 0, uint32_t textureId = 0);
	void Flush3DLines();

	CameraData m_camera;
	std::vector<MeshCommand> m_meshCommands;
	std::vector<LightData> m_lights;
	std::vector<LineCommand> m_lineCommands;

	uint32_t m_shader2D;
	uint32_t m_vao2D;
	uint32_t m_vbo2D;
	int32_t m_projectionLocation2D;
	int32_t m_renderModeLocation2D;
	int32_t m_textureLocation2D;

	uint32_t m_shaderLines3D;
	uint32_t m_vaoLines3D;
	uint32_t m_vboLines3D;
	int32_t m_projectionLocationLines3D;
	int32_t m_viewLocationLines3D;

	uint32_t m_nextFontId;
	std::unordered_map<uint32_t, std::unique_ptr<FontData>> m_fonts;
};
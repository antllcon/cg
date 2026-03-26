#pragma once
#include "src/core/interfaces/IMaterial.h"
#include "src/core/interfaces/IMesh.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/model/camera/CameraModel.h"
#include "src/model/entity/EntityModel.h"
#include "src/model/scene/SceneModel.h"
#include "src/view/IView.h"
#include <memory>
#include <vector>

struct EntityRenderData
{
	std::shared_ptr<EntityModel> model;
	std::shared_ptr<IMesh> mesh;
	std::shared_ptr<IMaterial> material;
};

class SceneView final
{
public:
	explicit SceneView(std::shared_ptr<SceneModel> sceneModel);

	void SetCamera(std::shared_ptr<CameraModel> camera);
	void AddUiView(std::shared_ptr<IView> view);
	void RegisterEntityVisuals(
		std::shared_ptr<EntityModel> model,
		std::shared_ptr<IMesh> mesh,
		std::shared_ptr<IMaterial> material);

	void Render(IRenderer& renderer) const;

private:
	std::shared_ptr<SceneModel> m_sceneModel;
	std::shared_ptr<CameraModel> m_cameraModel;
	std::vector<std::shared_ptr<IView>> m_uiViews;
	std::vector<EntityRenderData> m_entityVisuals;
};
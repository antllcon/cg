#pragma once

#include "IView.h"
#include "src/scene/controller/CameraController.h"
#include "src/scene/model/CameraModel.h"
#include "src/scene/model/RenderData.h"
#include "src/scene/model/SceneModel.h"
#include "stb_observer.h"
#include <memory>
#include <vector>

class View final
	: public IView
	, public Istb_observer<CameraState>
	, public Istb_observer<std::vector<SceneObject>>
{
public:
	View(
		std::shared_ptr<CameraModel> cameraModel,
		std::shared_ptr<SceneModel> sceneModel,
		std::shared_ptr<CameraController> cameraController);
	~View() override = default;

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;

	void Update(const CameraState& data, IObservable<CameraState>* subject) override;
	void Update(const std::vector<SceneObject>& data, IObservable<std::vector<SceneObject>>* subject) override;

private:
	std::shared_ptr<CameraModel> m_cameraModel;
	std::shared_ptr<SceneModel> m_sceneModel;
	std::shared_ptr<CameraController> m_cameraController;

	RenderData m_renderData;
};
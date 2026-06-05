#pragma once

#include "IView.h"
#include "src/scene/controller/CameraController.h"
#include "src/scene/model/CameraModel.h"
#include "src/scene/model/SceneModel.h"
#include <memory>

class View final : public IView
{
public:
	View(
		std::shared_ptr<CameraModel> cameraModel,
		std::shared_ptr<SceneModel> sceneModel,
		std::shared_ptr<CameraController> cameraController);
	~View() override = default;

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;

private:
	std::shared_ptr<CameraModel> m_cameraModel;
	std::shared_ptr<SceneModel> m_sceneModel;
	std::shared_ptr<CameraController> m_cameraController;
};
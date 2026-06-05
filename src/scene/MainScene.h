#pragma once

#include "controller/CameraController.h"
#include "controller/SceneController.h"
#include "model/CameraModel.h"
#include "model/SceneModel.h"
#include "src/utils/types/event/Event.h"
#include "view/View.h"
#include <memory>

class IRenderer;
class IWindow;

class MainScene final
{
public:
	explicit MainScene(IWindow& window);
	~MainScene() = default;

	void ProcessEvents(const Event& event);
	void Update(float dt);
	void Render(IRenderer& renderer) const;

private:
	std::shared_ptr<CameraModel> m_cameraModel;
	std::shared_ptr<SceneModel> m_sceneModel;
	std::shared_ptr<CameraController> m_cameraController;
	std::shared_ptr<SceneController> m_sceneController;
	std::shared_ptr<View> m_view;
};
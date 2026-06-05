#include "SceneController.h"

SceneController::SceneController(std::shared_ptr<SceneModel> sceneModel)
	: m_sceneModel(std::move(sceneModel))
{
}

void SceneController::Update(float dt)
{
	m_sceneModel->Update(dt);
}

void SceneController::HandleEvent(const Event& /*event*/)
{
}
#include "SceneController.h"

void SceneController::AddController(std::shared_ptr<IController> controller)
{
	if (controller)
	{
		m_controllers.push_back(std::move(controller));
	}
}

void SceneController::Update(float dt)
{
	for (const auto& controller : m_controllers)
	{
		controller->Update(dt);
	}
}

void SceneController::HandleEvent(const Event& event)
{
	for (const auto& controller : m_controllers)
	{
		controller->HandleEvent(event);
	}
}
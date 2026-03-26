#pragma once
#include "src/controller/IController.h"
#include "src/core/types/event/Event.h"
#include <memory>
#include <vector>

class SceneController
{
public:
	virtual ~SceneController() = default;

	void AddController(std::shared_ptr<IController> controller);

	void Update(float dt);
	void HandleEvent(const Event& event);

private:
	std::vector<std::shared_ptr<IController>> m_controllers;
};
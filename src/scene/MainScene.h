#pragma once

#include "controller/IController.h"
#include "model/Model.h"
#include "src/utils/types/event/Event.h"
#include "view/IView.h"
#include <memory>

class IRenderer;

class MainScene final
{
public:
	MainScene();
	~MainScene() = default;

	void ProcessEvents(const Event& event);
	void Update(float dt);
	void Render(IRenderer& renderer) const;

private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<IController> m_controller;
	std::shared_ptr<IView> m_view;
};
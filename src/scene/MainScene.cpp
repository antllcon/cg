#include "MainScene.h"
#include "controller/Controller.h"
#include "model/Model.h"
#include "src/system/renderer/IRenderer.h"
#include "view/View.h"
#include <stdexcept>

namespace
{
void AssertIsRendererValid(const IRenderer* renderer)
{
	if (!renderer)
	{
		throw std::runtime_error("Передан невалидный указатель на рендерер");
	}
}
} // namespace

MainScene::MainScene()
{
	auto model = std::make_shared<Model>();
	auto controller = std::make_shared<Controller>(model);
	auto view = std::make_shared<View>(model, controller);

	model->RegisterObserver(view);

	m_model = model;
	m_controller = controller;
	m_view = view;
}

void MainScene::ProcessEvents(const Event& event)
{
	m_view->HandleEvent(event);
}

void MainScene::Update(float dt)
{
	m_controller->Update(dt);
}

void MainScene::Render(IRenderer& renderer) const
{
	AssertIsRendererValid(&renderer);
	m_view->Render(renderer);
}
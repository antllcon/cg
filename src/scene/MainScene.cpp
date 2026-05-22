#include "MainScene.h"
#include "model/IModel.h"
#include "src/system/renderer/IRenderer.h"
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

	: m_model(std::make_shared<Model>())
	, m_controller(std::make_shared<Controller>())
	, m_view(std::make_shared<View>())
{
}

void MainScene::ProcessEvents(const Event& event)
{
	m_controller->HandleEvent(event);
	m_view->HandleEvent(event);
}

void MainScene::Update(float dt)
{
	m_model->UpdateState(dt);
	m_controller->Update(dt);
}

void MainScene::Render(IRenderer& renderer) const
{
	AssertIsRendererValid(&renderer);
	m_view->Render(renderer);
}
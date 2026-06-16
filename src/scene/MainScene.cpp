#include "MainScene.h"
#include "src/system/renderer/IRenderer.h"
#include "src/system/window/IWindow.h"
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

MainScene::MainScene(IWindow& window)
{
	m_cameraModel = std::make_shared<CameraModel>();
	m_sceneModel = std::make_shared<SceneModel>();
	m_cameraController = std::make_shared<CameraController>(m_cameraModel, window);
	m_sceneController = std::make_shared<SceneController>(m_sceneModel);
	m_view = std::make_shared<View>(m_cameraModel, m_sceneModel, m_cameraController);

	m_cameraModel->RegisterObserver(std::static_pointer_cast<Istb_observer<CameraState>>(m_view));
	m_sceneModel->RegisterObserver(std::static_pointer_cast<Istb_observer<std::vector<SceneObject>>>(m_view));
}

void MainScene::ProcessEvents(const Event& event)
{
	m_view->HandleEvent(event);
}

void MainScene::Update(float dt)
{
	m_cameraController->Update(dt);
	m_sceneController->Update(dt);
}

void MainScene::Render(IRenderer& renderer) const
{
	AssertIsRendererValid(&renderer);
	m_view->Render(renderer);
}
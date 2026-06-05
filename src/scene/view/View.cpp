#include "View.h"
#include "src/scene/model/RenderData.h"
#include "src/system/renderer/IRenderer.h"

View::View(
	std::shared_ptr<CameraModel> cameraModel,
	std::shared_ptr<SceneModel> sceneModel,
	std::shared_ptr<CameraController> cameraController)
	: m_cameraModel(std::move(cameraModel))
	, m_sceneModel(std::move(sceneModel))
	, m_cameraController(std::move(cameraController))
{
}

void View::HandleEvent(const Event& event)
{
	m_cameraController->HandleEvent(event);
}

void View::Render(IRenderer& renderer) const
{
	RenderData data{ m_cameraModel->GetState(), m_sceneModel->GetObjects() };
	renderer.RenderFrame(data);
}
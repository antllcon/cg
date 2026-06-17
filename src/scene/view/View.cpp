#include "View.h"
#include "src/system/renderer/IRenderer.h"

View::View(
	std::shared_ptr<CameraModel> cameraModel,
	std::shared_ptr<SceneModel> sceneModel,
	std::shared_ptr<CameraController> cameraController)
	: m_cameraModel(std::move(cameraModel))
	, m_sceneModel(std::move(sceneModel))
	, m_cameraController(std::move(cameraController))
{
	m_renderData.camera = m_cameraModel->GetState();
	m_renderData.objects = m_sceneModel->GetObjects();
	m_renderData.light = m_sceneModel->GetLight();
}

void View::HandleEvent(const Event& event)
{
	m_cameraController->HandleEvent(event);
}

void View::Render(IRenderer& renderer) const
{
	renderer.RenderFrame(m_renderData);
}

void View::Update(const CameraState& /*data*/, IObservable<CameraState>* /*subject*/)
{
	m_renderData.camera = m_cameraModel->GetState();
	m_renderData.light.position = m_renderData.camera.position;
}

void View::Update(const std::vector<SceneObject>& /*data*/, IObservable<std::vector<SceneObject>>* /*subject*/)
{
	m_renderData.objects = m_sceneModel->GetObjects();
	m_renderData.light = m_sceneModel->GetLight();
}
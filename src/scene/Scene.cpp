#include "Scene.h"

Scene::Scene()
	: m_sceneModel(std::make_shared<SceneModel>())
	, m_sceneController(std::make_shared<SceneController>())
{
	m_sceneView = std::make_shared<SceneView>(m_sceneModel);
}

void Scene::Init(IAudioManager&, IWindow&)
{
}

void Scene::ProcessEvents(const Event& event)
{
	m_sceneController->HandleEvent(event);
}

void Scene::Update(float dt)
{
	m_sceneController->Update(dt);
}

void Scene::Render(IRenderer& renderer) const
{
	m_sceneView->Render(renderer);
}

void Scene::OnException(const std::exception&)
{
}

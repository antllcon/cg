#include "Scene.h"

Scene::Scene()
	: m_sceneModel(std::make_shared<SceneModel>())
	, m_sceneController(std::make_shared<SceneController>())
{
}

void Scene::Init(std::shared_ptr<ThemeModel>, IAudioManager&, IWindow&)
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

void Scene::OnException(const std::exception&)
{
}
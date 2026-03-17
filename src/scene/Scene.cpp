#include "Scene.h"
#include "src/controller/IController.h"
#include "src/view/IView.h"

void Scene::Init(std::shared_ptr<ThemeModel>)
{
}

void Scene::ProcessEvents(const Event& event)
{
	for (auto& view : m_views)
	{
		view->HandleEvent(event);
	}
}

void Scene::Update(float dt)
{
	for (auto& controller : m_controllers)
	{
		controller->Update(dt);
	}
}

void Scene::Render(IRenderer& renderer) const
{
	for (auto& view : m_views)
	{
		view->Render(renderer);
	}
}

void Scene::AddView(std::shared_ptr<IView> view)
{
	m_views.push_back(std::move(view));
}

void Scene::AddController(std::shared_ptr<IController> controller)
{
	m_controllers.push_back(std::move(controller));
}

void Scene::OnException(const std::exception&)
{
}
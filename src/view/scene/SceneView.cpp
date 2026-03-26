#include "SceneView.h"

void SceneView::AddView(std::shared_ptr<IView> view)
{
	if (view)
	{
		m_views.push_back(std::move(view));
	}
}

void SceneView::Render(IRenderer& renderer) const
{
	for (const auto& view : m_views)
	{
		view->Render(renderer);
	}
}
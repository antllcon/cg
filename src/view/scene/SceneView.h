#pragma once
#include "src/core/interfaces/IRenderer.h"
#include "src/view/IView.h"
#include <memory>
#include <vector>

class SceneView
{
public:
	virtual ~SceneView() = default;

	void AddView(std::shared_ptr<IView> view);
	void Render(IRenderer& renderer) const;

private:
	std::vector<std::shared_ptr<IView>> m_views;
};
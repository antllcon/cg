#pragma once
#include "src/core/interfaces/IAudioManager.h"
#include "src/core/types/event/Event.h"
#include "src/model/theme/ThemeModel.h"
#include <memory>
#include <vector>

class IView;
class IController;
class IRenderer;

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager* audioManager);

	void ProcessEvents(const Event& event);
	void Update(float dt);
	void Render(IRenderer& renderer) const;
	virtual void OnException(const std::exception& e);

protected:
	template <typename T>
	void AddModel(std::shared_ptr<T> model)
	{
		m_models.push_back(model);
	}

	void AddView(std::shared_ptr<IView> view);
	void AddController(std::shared_ptr<IController> controller);

private:
	std::vector<std::shared_ptr<void>> m_models;
	std::vector<std::shared_ptr<IController>> m_controllers;
	std::vector<std::shared_ptr<IView>> m_views;
};
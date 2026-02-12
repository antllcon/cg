#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include <memory>
#include <vector>

class IView;
class IController;

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Init();

	void ProcessEvents(const sf::Event& event, const sf::RenderWindow& window);
	void Render(sf::RenderWindow& window) const;

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
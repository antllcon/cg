#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include <memory>

class IView;
class IController;
class CircleModel;

class Application final
{
public:
	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	void Run();

private:
	void ProcessEvents();
	void Render();
	void InitScene();

	sf::RenderWindow m_window;
	std::vector<std::shared_ptr<void>> m_models;
	std::vector<std::shared_ptr<IController>> m_controllers;
	std::vector<std::shared_ptr<IView>> m_views;
};
#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "src/scene/Scene.h"
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
	void Update(float dt);
	void Render();
	void LoadScene(std::unique_ptr<Scene> scene);

	sf::RenderWindow m_window;
	std::unique_ptr<Scene> m_scene;
};
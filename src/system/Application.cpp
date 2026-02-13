#include "Application.h"
#include "Monitor.h"
#include "Windows.h"
#include "src/scene/MainScene.h"

namespace
{
constexpr unsigned int WINDOW_WIDTH = 800u;
constexpr unsigned int WINDOW_HEIGHT = 600u;
constexpr auto WINDOW_NAME = "MVC template";
constexpr sf::Color BACKGROUND_COLOR = sf::Color::Black;
} // namespace

Application::Application()
	: m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), WINDOW_NAME)
{
	auto refreshRate = Monitor::GetMonitorRefreshRate();
	m_window.setFramerateLimit(refreshRate);
	LoadScene(std::make_unique<MainScene>());
}

Application::~Application()
{
}

void Application::Run()
{
	sf::Clock clock;
	while (m_window.isOpen())
	{
		auto dt = clock.restart();

		ProcessEvents();
		Update(dt.asSeconds());
		Render();
	}
}

void Application::ProcessEvents()
{
	while (const auto event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}

		if (m_scene)
		{
			m_scene->ProcessEvents(*event, m_window);
		}
	}
}

void Application::Update(float dt)
{
	if (m_scene)
	{
		m_scene->Update(dt);
	}
}

void Application::Render()
{
	m_window.clear(BACKGROUND_COLOR);

	if (m_scene)
	{
		m_scene->Render(m_window);
	}

	m_window.display();
}

void Application::LoadScene(std::unique_ptr<Scene> scene)
{
	m_scene = std::move(scene);
	if (m_scene)
	{
		m_scene->Init();
	}
}
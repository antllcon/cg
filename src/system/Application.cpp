#include "Application.h"
#include "AppConfig.h"
#include "src/scene/MainScene.h"

Application::Application()
	: m_window(sf::VideoMode({AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT}), AppConfig::WINDOW_NAME)
{
	auto refreshRate = AppConfig::GetMonitorRefreshRate();
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
			try
			{
				m_scene->ProcessEvents(*event, m_window);
			}
			catch (const std::exception& e)
			{
				m_scene->OnException(e);
			}
		}
	}
}

void Application::Update(float dt)
{
	if (m_scene)
	{
		try
		{
			m_scene->Update(dt);
		}
		catch (const std::exception& e)
		{
			m_scene->OnException(e);
		}
	}
}

void Application::Render()
{
	m_window.clear(AppConfig::BACKGROUND_COLOR);

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
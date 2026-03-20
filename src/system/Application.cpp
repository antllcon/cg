#include "Application.h"
#include "AppConfig.h"
#include "src/core/types/event/EventHandling.h"
#include "src/scene/main/MainScene.h"
#include <chrono>
#include <filesystem>
#include <stdexcept>

namespace
{
void AssertIsWindowValid(const IWindow* window)
{
	if (!window)
	{
		throw std::runtime_error("Окно не передано в приложение");
	}

	if (!window->IsOpen())
	{
		throw std::runtime_error("Окно не инициализировано для применения системных настроек");
	}
}

void SetupWindowProperties(IWindow& window)
{
	window.SetTitleBarTheme(true);
	window.SetVSync(true);
	// window.SetIconColor(AppConfig::ICON_COLOR_LIGHT);
	window.SetIconFromFile("static/icon.png");
}
} // namespace

Application::Application(
	std::unique_ptr<IWindow> window,
	std::unique_ptr<IRenderer> renderer,
	std::unique_ptr<IAudioManager> audioManager)
	: m_window(std::move(window))
	, m_renderer(std::move(renderer))
	, m_audioManager(std::move(audioManager))
	, m_themeModel(std::make_shared<ThemeModel>())
{
	AssertIsWindowValid(m_window.get());
	SetupWindowProperties(*m_window);
}

void Application::Init()
{
	m_themeModel->RegisterObserver(shared_from_this());
	LoadScene(std::make_unique<MainScene>());
}

void Application::Run()
{
	auto lastTime = std::chrono::steady_clock::now();

	while (m_window->IsOpen())
	{
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> dt = currentTime - lastTime;
		lastTime = currentTime;

		ProcessEvents();
		UpdateLogic(dt.count());
		Render();
	}
}

void Application::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	if (m_window)
	{
		m_window->SetTitleBarTheme(data.isDark);
		Color iconColor = data.isDark ? AppConfig::ICON_COLOR_LIGHT : AppConfig::ICON_COLOR_DARK;
		m_window->SetIconColor(iconColor);
	}
}

void Application::ProcessEvents()
{
	while (std::optional<Event> eventOpt = m_window->PollEvent())
	{
		const Event& event = eventOpt.value();

		std::visit(Overload{
					   [this](const WindowClosedEvent&) {
						   m_window->Close();
					   },
					   [](const auto&) {
					   }},
			event);

		if (m_scene)
		{
			try
			{
				m_scene->ProcessEvents(event);
			}
			catch (const std::exception& e)
			{
				m_scene->OnException(e);
			}
		}
	}
}

void Application::UpdateLogic(float dt)
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
	m_renderer->Clear(m_themeModel->GetData().windowBackground);

	if (m_scene)
	{
		m_scene->Render(*m_renderer);
	}

	m_renderer->Display();
}

void Application::LoadScene(std::unique_ptr<Scene> scene)
{
	m_scene = std::move(scene);

	if (m_scene)
	{
		m_scene->Init(m_themeModel);
	}
}
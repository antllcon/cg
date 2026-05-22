#include "Application.h"
#include "src/system/AppConfig.h"
#include "src/system/window/GlfwWindow.h"
#include <chrono>
#include <stdexcept>
#include <variant>

namespace
{
using Clock = std::chrono::high_resolution_clock;

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

void AssertIsRendererValid(const IRenderer* renderer)
{
	if (!renderer)
	{
		throw std::runtime_error("Рендерер не инициализирован");
	}
}

void SetupWindowProperties(IWindow& window)
{
	window.SetTitleBarColor(AppConfig::DARK_ENABLED);
	window.SetIcon(AppConfig::ICON_SIZE, AppConfig::ICON_COLOR);
	window.SetVSync(AppConfig::VSYNC_ENABLED);
}

void SetupRendererProperties(IRenderer& m_renderer)
{
	m_renderer.SetClearColor(AppConfig::WINDOW_BG);
}

float ConsumeDeltaTime(Clock::time_point& lastTime)
{
	const auto currentTime = Clock::now();
	const auto duration = currentTime - lastTime;
	lastTime = currentTime;
	return duration.count();
}
} // namespace

Application::Application()
	: m_window(std::make_unique<GlfwWindow>())
	, m_renderer(std::make_unique<OpenGLRenderer>())
	, m_mainScene(std::make_unique<MainScene>())
{
	AssertIsWindowValid(m_window.get());
	AssertIsRendererValid(m_renderer.get());

	SetupWindowProperties(*m_window);
	SetupRendererProperties(*m_renderer);
}

void Application::Run()
{
	auto lastTime = Clock::now();

	while (m_window->IsOpen())
	{
		const auto dt = ConsumeDeltaTime(lastTime);

		ProcessEvents();
		UpdateLogic(dt);
		Render();
	}
}

void Application::ProcessEvents()
{
	while (auto eventOpt = m_window->PollEvent())
	{
		const auto& event = eventOpt.value();

		if (std::holds_alternative<WindowClosedEvent>(event))
		{
			m_window->Close();
		}

		m_mainScene->ProcessEvents(event);
	}
}

void Application::UpdateLogic(float dt)
{
	m_mainScene->Update(dt);
}

void Application::Render()
{
	m_renderer->Clear();
	m_mainScene->Render(*m_renderer);
	m_renderer->Display();
}
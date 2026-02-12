#include "Application.h"

#include "Monitor.h"
#include "Windows.h"
#include "src/controller/CircleController.h"
#include "src/model/CircleModel.h"
#include "src/view/CircleView.h"

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
	InitScene();
}

Application::~Application()
{
}

void Application::Run()
{
	while (m_window.isOpen())
	{
		ProcessEvents();
		Render();
	}
}

void Application::InitScene()
{
	auto circleModel = std::make_shared<CircleModel>();
	m_models.push_back(circleModel);

	auto circleController = std::make_shared<CircleController>(circleModel);
	m_controllers.push_back(circleController);

	auto circleView = std::make_shared<CircleView>(circleController, circleModel);
	m_views.push_back(circleView);
	circleModel->RegisterObserver(circleView);
}

void Application::ProcessEvents()
{
	while (const auto event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}

		for (auto& view : m_views)
		{
			view->HandleEvent(*event, m_window);
		}
	}
}

void Application::Render()
{
	m_window.clear(BACKGROUND_COLOR);

	for (const auto& view : m_views)
	{
		view->Render(m_window);
	}

	m_window.display();
}
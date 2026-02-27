#include "SimulationView.h"
#include "SFML/Window/Event.hpp"
#include "src/controller/simulation/SimulationController.h"
#include <cmath>

namespace
{
	constexpr float CANNON_LENGTH = 50.0f;
	constexpr float CANNON_THICKNESS = 16.0f;

	sf::Angle CalculateRotationAngle(const sf::Vector2f& direction)
	{
		return sf::radians(std::atan2(direction.y, direction.x));
	}

	sf::RectangleShape CreateObstacleShape(const RectObstacleData& data)
	{
		sf::RectangleShape shape(data.size);
		shape.setOrigin({data.size.x / 2.0f, data.size.y / 2.0f});
		shape.setPosition(data.center);
		shape.setRotation(sf::radians(data.rotationAngle));
		shape.setFillColor(sf::Color(120, 120, 120));

		return shape;
	}

	sf::CircleShape CreateBallShape(const BallData& data)
	{
		sf::CircleShape shape(data.radius);
		shape.setOrigin({data.radius, data.radius});
		shape.setPosition(data.position);
		shape.setFillColor(sf::Color(231, 76, 60));

		return shape;
	}

	void SetupCannonShape(sf::RectangleShape& shape, const CannonData& data)
	{
		shape.setSize({CANNON_LENGTH, CANNON_THICKNESS});
		shape.setOrigin({0.0f, CANNON_THICKNESS / 2.0f});
		shape.setPosition(data.position);
		shape.setRotation(CalculateRotationAngle(data.targetDirection));
		shape.setFillColor(sf::Color(46, 204, 113));
	}
}

SimulationView::SimulationView(std::shared_ptr<SimulationModel> model, std::shared_ptr<SimulationController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void SimulationView::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>())
	{
		m_controller->OnMouseMoved(
			static_cast<float>(mouseMove->position.x),
			static_cast<float>(mouseMove->position.y)
		);
	}
	else if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButton->button == sf::Mouse::Button::Left)
		{
			m_controller->OnCanvasClicked();
		}
	}
	else if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyEvent->code == sf::Keyboard::Key::Right)
		{
			m_controller->OnRightArrowPressed();
		}
		else if (keyEvent->code == sf::Keyboard::Key::Left)
		{
			m_controller->OnLeftArrowPressed();
		}
	}
}

void SimulationView::Render(sf::RenderWindow& window) const
{
	for (const auto& obstacle : m_obstacles)
	{
		window.draw(obstacle);
	}

	for (const auto& ball : m_balls)
	{
		window.draw(ball);
	}

	window.draw(m_cannon);
}

void SimulationView::Update(const SimulationData& data, IObservable<SimulationData>*)
{
	UpdateObstacles(data.obstacles);
	UpdateCannon(data.cannon);
	UpdateBalls(data.balls);
}

void SimulationView::UpdateBalls(const std::vector<BallData>& balls)
{
	m_balls.clear();
	m_balls.reserve(balls.size());

	for (const auto& ballData : balls)
	{
		m_balls.push_back(CreateBallShape(ballData));
	}
}

void SimulationView::UpdateObstacles(const std::vector<RectObstacleData>& obstacles)
{
	m_obstacles.clear();
	m_obstacles.reserve(obstacles.size());

	for (const auto& obstacleData : obstacles)
	{
		m_obstacles.push_back(CreateObstacleShape(obstacleData));
	}
}

void SimulationView::UpdateCannon(const CannonData& cannon)
{
	SetupCannonShape(m_cannon, cannon);
}
#include "AsteroidsView.h"
#include "src/controller/asteroids/AsteroidsController.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/core/types/Event.h"
#include <cmath>

namespace
{
void TransformPolygon(std::vector<Point2f>& points, const Point2f& position, float angle)
{
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	for (auto& p : points)
	{
		float rx = p.x * cosA - p.y * sinA;
		float ry = p.x * sinA + p.y * cosA;

		p.x = rx + position.x;
		p.y = ry + position.y;
	}
}

std::vector<Point2f> GetShipVertices()
{
	return {
		{20.0f, 0.0f},
		{-15.0f, -12.0f},
		{-10.0f, 0.0f},
		{-15.0f, 12.0f}};
}

std::vector<Point2f> GetFlameVertices()
{
	return {
		{-10.0f, 0.0f},
		{-15.0f, -6.0f},
		{-25.0f, 0.0f},
		{-15.0f, 6.0f}};
}
} // namespace

AsteroidsView::AsteroidsView(std::shared_ptr<AsteroidsModel> model, std::shared_ptr<AsteroidsController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void AsteroidsView::HandleEvent(const Event& event)
{
	if (event.type == EventType::KeyPressed)
	{
		if (event.key.code == KeyCode::Space)
		{
			m_controller->OnShootPressed();
		}
	}

	if (event.type == EventType::KeyPressed || event.type == EventType::KeyReleased)
	{
		bool isPressed = (event.type == EventType::KeyPressed);

		if (event.key.code == KeyCode::Escape)
		{
			// Настроить выход или паузу
		}
		else if (event.key.code == KeyCode::Up)
		{
			if (isPressed)
			{
				m_controller->OnAcceleratePressed();
			}
			else
			{
				m_controller->OnAccelerateReleased();
			}
		}
		else if (event.key.code == KeyCode::Left)
		{
			if (isPressed)
			{
				m_controller->OnRotateLeftPressed();
			}
			else
			{
				m_controller->OnRotateLeftReleased();
			}
		}
		else if (event.key.code == KeyCode::Right)
		{

			if (isPressed)
			{
				m_controller->OnRotateRightPressed();
			}
			else
			{
				m_controller->OnRotateRightReleased();
			}
		}
	}
}

void AsteroidsView::Render(IRenderer& renderer) const
{
	if (m_data.ship.state == ShipState::Alive)
	{
		std::vector<Point2f> shipVertices = GetShipVertices();
		TransformPolygon(shipVertices, m_data.ship.position, m_data.ship.angle);
		renderer.DrawPolygon(shipVertices, {200, 200, 200, 255});

		if (m_data.ship.isThrusting)
		{
			std::vector<Point2f> flameVertices = GetFlameVertices();
			TransformPolygon(flameVertices, m_data.ship.position, m_data.ship.angle);
			renderer.DrawPolygon(flameVertices, {255, 100, 0, 255});
		}
	}
}

void AsteroidsView::Update(const AsteroidsData& data, IObservable<AsteroidsData>*)
{
	m_data = data;
}
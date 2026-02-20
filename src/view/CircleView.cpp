#include "CircleView.h"
#include "SFML/Window/Event.hpp"
#include "src/controller/CircleController.h"

namespace
{
	void AddSymmetricPoints(std::vector<sf::Vector2i>& points, int xc, int yc, int x, int y)
	{
		points.push_back({xc + x, yc + y});
		points.push_back({xc - x, yc + y});
		points.push_back({xc + x, yc - y});
		points.push_back({xc - x, yc - y});
		points.push_back({xc + y, yc + x});
		points.push_back({xc - y, yc + x});
		points.push_back({xc + y, yc - x});
		points.push_back({xc - y, yc - x});
	}

	std::vector<sf::Vector2i> CalculateBresenham(int xc, int yc, int r)
	{
		std::vector<sf::Vector2i> points;

		int x = 0;
		int y = r;
		int d = 3 - 2 * r;

		while (x <= y)
		{
			AddSymmetricPoints(points, xc, yc, x, y);

			if (d < 0)
			{
				d = d + 4 * x + 6;
			}
			else
			{
				y = y - 1;
				d = d + 4 * (x - y) + 10;
			}

			x = x + 1;
		}

		return points;
	}
}

CircleView::CircleView(
	std::shared_ptr<CircleModel> model,
	std::shared_ptr<CircleController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void CircleView::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButton->button == sf::Mouse::Button::Left)
		{
			m_controller->OnCanvasClicked(
				static_cast<float>(mouseButton->position.x),
				static_cast<float>(mouseButton->position.y));
		}
	}
	else if (const auto* mouseScroll = event.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (mouseScroll->wheel == sf::Mouse::Wheel::Vertical)
		{
			m_controller->OnMouseScrolled(
				static_cast<float>(mouseScroll->position.x),
				static_cast<float>(mouseScroll->position.y),
				mouseScroll->delta);
		}
	}
}

void CircleView::Render(sf::RenderWindow& window) const
{
	window.draw(m_vertices);
}

void CircleView::Update(const CircleData& data, IObservable<CircleData>*)
{
	std::vector<sf::Vector2i> rasterPoints = CalculateBresenham(data.center.x, data.center.y, data.radius);
	UpdateVertices(rasterPoints, data.color);
}

void CircleView::UpdateVertices(const std::vector<sf::Vector2i>& points, const sf::Color& color)
{
	m_vertices.setPrimitiveType(sf::PrimitiveType::Points);
	m_vertices.resize(points.size());

	for (size_t i = 0; i < points.size(); ++i)
	{
		m_vertices[i].position = sf::Vector2f(
			static_cast<float>(points[i].x),
			static_cast<float>(points[i].y));
		m_vertices[i].color = color;
	}
}
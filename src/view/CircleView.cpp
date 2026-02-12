#include "CircleView.h"

namespace
{
bool IsPointInside(const sf::Shape& shape, const sf::Vector2i& point)
{
	return shape.getGlobalBounds().contains({static_cast<float>(point.x), static_cast<float>(point.y)});
}
} // namespace

CircleView::CircleView(std::shared_ptr<CircleController> controller, std::shared_ptr<CircleModel> model)
	: m_controller(std::move(controller))
{
	m_circle.setFillColor(sf::Color::Cyan);

	if (model)
	{
		Update({model->GetRadius()}, nullptr);
	}
}

void CircleView::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
	{
		const sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		if (mouseEvent->button == sf::Mouse::Button::Left)
		{
			if (IsPointInside(m_circle, mousePos))
			{
				m_controller->IncRadius();
			}
		}
		if (mouseEvent->button == sf::Mouse::Button::Right)
		{
			if (IsPointInside(m_circle, mousePos))
			{
				m_controller->DecRadius();
			}
		}
	}
}

void CircleView::Render(sf::RenderWindow& window) const
{
	window.draw(m_circle);
}

void CircleView::Update(CircleData const& data, IObservable<CircleData>*)
{
	m_circle.setRadius(data.radius);
	m_circle.setOrigin({data.radius, data.radius});
	m_circle.setPosition({400.f, 300.f});
}
#include "CircleView.h"
#include "SFML/Window/Event.hpp"
#include "src/controller/CircleController.h"

namespace
{
	sf::Color BlendColors(const sf::Color& c1, const sf::Color& c2, float t)
	{
		float clampedT = std::max(0.0f, std::min(1.0f, t));

		return sf::Color(
			static_cast<uint8_t>(c1.r + clampedT * (c2.r - c1.r)),
			static_cast<uint8_t>(c1.g + clampedT * (c2.g - c1.g)),
			static_cast<uint8_t>(c1.b + clampedT * (c2.b - c1.b)),
			static_cast<uint8_t>(c1.a + clampedT * (c2.a - c1.a))
		);
	}

	void AddSymmetricVertices(std::vector<sf::Vertex>& vertices, int xc, int yc, int x, int y, const sf::Color& color)
	{
		vertices.push_back(sf::Vertex(sf::Vector2f(static_cast<float>(xc + x), static_cast<float>(yc + y)), color));

		if (x != 0)
		{
			vertices.push_back(sf::Vertex(sf::Vector2f(static_cast<float>(xc - x), static_cast<float>(yc + y)), color));
		}

		if (y != 0)
		{
			vertices.push_back(sf::Vertex(sf::Vector2f(static_cast<float>(xc + x), static_cast<float>(yc - y)), color));
		}

		if (x != 0 && y != 0)
		{
			vertices.push_back(sf::Vertex(sf::Vector2f(static_cast<float>(xc - x), static_cast<float>(yc - y)), color));
		}
	}

	std::vector<sf::Vertex> CalculateSmoothCircle(const CircleData& data)
	{
		std::vector<sf::Vertex> vertices;

		float rOut = static_cast<float>(data.radius);
		float rIn = rOut - static_cast<float>(data.thickness);
		int rMax = static_cast<int>(std::ceil(rOut + 1.0f));

		for (int y = 0; y <= rMax; ++y)
		{
			for (int x = 0; x <= rMax; ++x)
			{
				float d = std::hypot(static_cast<float>(x), static_cast<float>(y));

				if (d > rOut + 0.5f)
				{
					break;
				}

				sf::Color pixelColor = sf::Color::Transparent;

				if (d <= rIn - 0.5f)
				{
					if (data.isFilled)
					{
						pixelColor = data.fillColor;
					}
				}
				else if (d < rIn + 0.5f)
				{
					float t = d - (rIn - 0.5f);

					if (data.isFilled)
					{
						pixelColor = BlendColors(data.fillColor, data.outlineColor, t);
					}
					else
					{
						pixelColor = BlendColors(sf::Color::Transparent, data.outlineColor, t);
					}
				}
				else if (d <= rOut - 0.5f)
				{
					pixelColor = data.outlineColor;
				}
				else if (d < rOut + 0.5f)
				{
					float t = (rOut + 0.5f) - d;
					pixelColor = data.outlineColor;
					pixelColor.a = static_cast<uint8_t>(data.outlineColor.a * t);
				}

				if (pixelColor.a > 0)
				{
					AddSymmetricVertices(vertices, data.center.x, data.center.y, x, y, pixelColor);
				}
			}
		}

		return vertices;
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
		bool isRightClick = (mouseButton->button == sf::Mouse::Button::Right);

		if (mouseButton->button == sf::Mouse::Button::Left)
		{
			m_controller->OnCanvasClicked(
				static_cast<float>(mouseButton->position.x),
				static_cast<float>(mouseButton->position.y),
				isRightClick);
		}
	}
	else if (const auto* mouseScroll = event.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (mouseScroll->wheel == sf::Mouse::Wheel::Vertical)
		{
			bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);

			m_controller->OnMouseScrolled(
				static_cast<float>(mouseScroll->position.x),
				static_cast<float>(mouseScroll->position.y),
				mouseScroll->delta,
				isShiftPressed);
		}
	}
}

void CircleView::Render(sf::RenderWindow& window) const
{
	window.draw(m_vertices);
}

void CircleView::Update(const CircleData& data, IObservable<CircleData>*)
{
	std::vector<sf::Vertex> rasterPoints = CalculateSmoothCircle(data);
	UpdateVertices(rasterPoints);
}

void CircleView::UpdateVertices(const std::vector<sf::Vertex>& vertices)
{
	m_vertices.setPrimitiveType(sf::PrimitiveType::Points);
	m_vertices.resize(vertices.size());

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		m_vertices[i] = vertices[i];
	}
}
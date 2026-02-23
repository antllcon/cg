#include "CircleView.h"
#include "../../controller/circle/CircleController.h"
#include "SFML/Window/Event.hpp"
#include "src/system/AppConfig.h"

namespace
{
constexpr float HALF_PIXEL = 0.5f;

sf::Color BlendColors(const sf::Color& color1, const sf::Color& color2, float blendFactor)
{
	float clampedFactor = std::max(0.0f, std::min(1.0f, blendFactor));

	return sf::Color(
		static_cast<uint8_t>(color1.r + clampedFactor * (color2.r - color1.r)),
		static_cast<uint8_t>(color1.g + clampedFactor * (color2.g - color1.g)),
		static_cast<uint8_t>(color1.b + clampedFactor * (color2.b - color1.b)),
		static_cast<uint8_t>(color1.a + clampedFactor * (color2.a - color1.a)));
}

void AddSymmetricVertices(std::vector<sf::Vertex>& vertices, int centerX, int centerY, int x, int y, const sf::Color& color)
{
	auto addIfVisible = [&](int px, int py) {
		if (px >= 0 && px <= AppConfig::WINDOW_WIDTH && py >= 0 && py <= AppConfig::WINDOW_HEIGHT)
		{
			vertices.push_back(sf::Vertex(sf::Vector2f(static_cast<float>(px), static_cast<float>(py)), color));
		}
	};

	addIfVisible(centerX + x, centerY + y);

	if (x != 0)
	{
		addIfVisible(centerX - x, centerY + y);
	}

	if (y != 0)
	{
		addIfVisible(centerX + x, centerY - y);
	}

	if (x != 0 && y != 0)
	{
		addIfVisible(centerX - x, centerY - y);
	}
}

sf::Color CalculateSolidPixelColor(float distanceFromCenter, float radius, const sf::Color& fillColor)
{
	if (distanceFromCenter <= radius - HALF_PIXEL)
	{
		return fillColor;
	}

	if (distanceFromCenter < radius + HALF_PIXEL)
	{
		float coverage = (radius + HALF_PIXEL) - distanceFromCenter;
		sf::Color resultColor = fillColor;
		resultColor.a = static_cast<uint8_t>(fillColor.a * coverage);

		return resultColor;
	}

	return sf::Color::Transparent;
}

sf::Color CalculateOutlinedPixelColor(float distanceFromCenter, float innerRadius, float outerRadius, const sf::Color& fillColor, const sf::Color& outlineColor)
{
	if (distanceFromCenter <= innerRadius - HALF_PIXEL)
	{
		return fillColor;
	}

	if (distanceFromCenter < innerRadius + HALF_PIXEL)
	{
		float blendFactor = distanceFromCenter - (innerRadius - HALF_PIXEL);

		return BlendColors(fillColor, outlineColor, blendFactor);
	}

	if (distanceFromCenter <= outerRadius - HALF_PIXEL)
	{
		return outlineColor;
	}

	if (distanceFromCenter < outerRadius + HALF_PIXEL)
	{
		float coverage = (outerRadius + HALF_PIXEL) - distanceFromCenter;
		sf::Color resultColor = outlineColor;
		resultColor.a = static_cast<uint8_t>(outlineColor.a * coverage);

		return resultColor;
	}

	return sf::Color::Transparent;
}

std::vector<sf::Vertex> CalculateSmoothCircle(const CircleData& data)
{
	std::vector<sf::Vertex> vertices;

	float innerRadius = static_cast<float>(data.radius);
	float outerRadius = innerRadius + static_cast<float>(data.thickness);
	int boundingBoxRadius = static_cast<int>(std::ceil(outerRadius + 1.0f));

	for (int y = 0; y <= boundingBoxRadius; ++y)
	{
		for (int x = 0; x <= boundingBoxRadius; ++x)
		{
			float distanceFromCenter = std::hypot(static_cast<float>(x), static_cast<float>(y));

			if (distanceFromCenter > outerRadius + HALF_PIXEL)
			{
				break;
			}

			sf::Color pixelColor = sf::Color::Transparent;

			if (data.thickness == 0)
			{
				pixelColor = CalculateSolidPixelColor(distanceFromCenter, outerRadius, data.fillColor);
			}
			else
			{
				pixelColor = CalculateOutlinedPixelColor(distanceFromCenter, innerRadius, outerRadius, data.fillColor, data.thicknessColor);
			}

			if (pixelColor.a > 0)
			{
				AddSymmetricVertices(vertices, data.center.x, data.center.y, x, y, pixelColor);
			}
		}
	}

	return vertices;
}
} // namespace

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
		if (mouseButton->button == sf::Mouse::Button::Left || mouseButton->button == sf::Mouse::Button::Right)
		{
			bool isRightClick = (mouseButton->button == sf::Mouse::Button::Right);

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
	UpdateVertices(CalculateSmoothCircle(data));
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
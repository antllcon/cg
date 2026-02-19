#include "ShapeFactory.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

namespace ShapeFactory
{
std::unique_ptr<sf::Drawable> CreateRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
	auto rect = std::make_unique<sf::RectangleShape>(size);
	rect->setPosition(pos);
	rect->setFillColor(color);
	return rect;
}

std::unique_ptr<sf::Drawable> CreateCircle(const sf::Vector2f& pos, float radius, const sf::Color& color)
{
	auto circle = std::make_unique<sf::CircleShape>(radius);
	circle->setPosition(pos);
	circle->setFillColor(color);
	return circle;
}
}
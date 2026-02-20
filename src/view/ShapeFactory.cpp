#include "ShapeFactory.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace ShapeFactory
{
std::unique_ptr<sf::Drawable> CreateRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& fill, const sf::Color& outline, float outlineThickness)
{
	auto rect = std::make_unique<sf::RectangleShape>(size);
	rect->setPosition(pos);
	rect->setFillColor(fill);
	rect->setOutlineColor(outline);
	rect->setOutlineThickness(outlineThickness);
	return rect;
}

std::unique_ptr<sf::Drawable> CreatePolygon(const sf::Vector2f& pos, float radius, size_t points, const sf::Color& fill, const sf::Color& outline, float outlineThickness)
{
	auto polygon = std::make_unique<sf::CircleShape>(radius, points);
	polygon->setPosition(pos);
	polygon->setFillColor(fill);
	polygon->setOutlineColor(outline);
	polygon->setOutlineThickness(outlineThickness);
	return polygon;
}
} // namespace ShapeFactory
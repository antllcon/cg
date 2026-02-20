#include "ShapeFactory.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <stdexcept>

namespace
{
void AssertIsNotEmpty(size_t size)
{
	if (size == 0)
	{
		throw std::runtime_error("Массив точек для фигуры пуст");
	}
}
}

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

std::unique_ptr<sf::Drawable> CreatePathShape(const sf::Vector2f& pos, const std::vector<sf::Vector2f>& points, const sf::Color& fill, const sf::Color& outline, float outlineThickness)
{
	AssertIsNotEmpty(points.size());

	auto shape = std::make_unique<sf::ConvexShape>(points.size());

	for (size_t i = 0; i < points.size(); ++i)
	{
		shape->setPoint(i, points[i]);
	}

	shape->setPosition(pos);
	shape->setFillColor(fill);
	shape->setOutlineColor(outline);
	shape->setOutlineThickness(outlineThickness);

	return shape;
}
}
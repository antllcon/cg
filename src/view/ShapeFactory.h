#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/System/Vector2.hpp"
#include <memory>
#include <vector>

namespace ShapeFactory
{
std::unique_ptr<sf::Drawable> CreateRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& fill, const sf::Color& outline = sf::Color::Transparent, float outlineThickness = 0.0f);
std::unique_ptr<sf::Drawable> CreatePolygon(const sf::Vector2f& pos, float radius, size_t points, const sf::Color& fill, const sf::Color& outline = sf::Color::Transparent, float outlineThickness = 0.0f);
std::unique_ptr<sf::Drawable> CreatePathShape(const sf::Vector2f& pos, const std::vector<sf::Vector2f>& points, const sf::Color& fill, const sf::Color& outline = sf::Color::Transparent, float outlineThickness = 0.0f);
}
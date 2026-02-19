#pragma once
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include <memory>

namespace ShapeFactory
{
std::unique_ptr<sf::Drawable> CreateRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
std::unique_ptr<sf::Drawable> CreateCircle(const sf::Vector2f& pos, float radius, const sf::Color& color);
}
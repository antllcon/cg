#include "CircleModel.h"
#include <algorithm>
#include <stdexcept>

namespace
{
constexpr int MIN_RADIUS = 5;

void AssertIsRadiusValid(int radius)
{
	if (radius < MIN_RADIUS)
	{
		throw std::runtime_error("Радиус не может быть меньше минимально допустимого значения");
	}
}
}

CircleModel::CircleModel()
{
	m_data.center = {200, 200};
	m_data.radius = 50;
	m_data.color = sf::Color::White;
}

void CircleModel::SetCenter(const sf::Vector2i& center)
{
	m_data.center = center;
	NotifyObservers();
}

void CircleModel::ChangeRadius(int delta)
{
	int newRadius = m_data.radius + delta;
	newRadius = std::max(MIN_RADIUS, newRadius);

	AssertIsRadiusValid(newRadius);

	m_data.radius = newRadius;
	NotifyObservers();
}

const CircleData& CircleModel::GetData() const
{
	return m_data;
}

CircleData CircleModel::GetChangedData() const
{
	return m_data;
}
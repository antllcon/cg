#include "CircleModel.h"
#include <stdexcept>

namespace
{
void AssertIsPositive(int value)
{
	if (value <= 0)
	{
		throw std::runtime_error("Значение шага должно быть положительным");
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

void CircleModel::IncreaseRadius(int step)
{
	AssertIsPositive(step);
	m_data.radius += step;
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
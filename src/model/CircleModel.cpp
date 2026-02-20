#include "CircleModel.h"
#include <algorithm>
#include <stdexcept>

namespace
{
constexpr int MIN_RADIUS = 5;
constexpr int MIN_THICKNESS = 1;
}

CircleModel::CircleModel()
{
	m_data.center = {200, 200};
	m_data.radius = 50;
	m_data.thickness = 10;
	m_data.outlineColor = sf::Color::White;
	m_data.fillColor = sf::Color(46, 204, 113);
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

	m_data.radius = newRadius;
	m_data.thickness = std::min(m_data.thickness, m_data.radius);

	NotifyObservers();
}

void CircleModel::ChangeThickness(int delta)
{
	int newThickness = m_data.thickness + delta;
	newThickness = std::max(MIN_THICKNESS, newThickness);
	newThickness = std::min(m_data.radius, newThickness);

	m_data.thickness = newThickness;

	NotifyObservers();
}

void CircleModel::ToggleFill()
{
	m_data.isFilled = !m_data.isFilled;
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
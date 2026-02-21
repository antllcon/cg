#include "CircleModel.h"
#include <random>

namespace
{
constexpr int MIN_RADIUS = 5;
constexpr int MIN_THICKNESS = 0;

sf::Color GenerateRandomColor()
{
	static std::mt19937 rng(std::random_device{}());
	static std::uniform_int_distribution<int> dist(0, 255);

	return sf::Color(
		static_cast<uint8_t>(dist(rng)),
		static_cast<uint8_t>(dist(rng)),
		static_cast<uint8_t>(dist(rng)),
		static_cast<uint8_t>(dist(rng))
	);
}
} // namespace

CircleModel::CircleModel()
{
	m_data.center = {200, 200};
	m_data.radius = 50;
	m_data.thickness = 10;
	m_data.thicknessColor = sf::Color::White;
	m_data.fillColor = sf::Color(46, 204, 113);
}

void CircleModel::SetCenter(const sf::Vector2i& center)
{
	m_data.center = center;
	NotifyObservers();
}

void CircleModel::ChangeRadius(int delta)
{
	if (m_data.radius + delta >= MIN_RADIUS)
	{
		m_data.radius += delta;
	}

	NotifyObservers();
}

void CircleModel::ChangeThickness(int delta)
{
	if (m_data.thickness + delta >= MIN_THICKNESS)
	{
		m_data.thickness += delta;
	}

	NotifyObservers();
}

void CircleModel::RandomFillColor()
{
	m_data.fillColor = GenerateRandomColor();
	NotifyObservers();
}

void CircleModel::RandomThicknessColor()
{
	m_data.thicknessColor = GenerateRandomColor();
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
#include "CircleModel.h"
#include <random>

namespace
{
constexpr float MIN_RADIUS = 5.0f;
constexpr float MIN_THICKNESS = 0.0f;

Color GenerateRandomColor()
{
	static std::mt19937 rng(std::random_device{}());
	static std::uniform_int_distribution<int> dist(0, 255);

	return {
		static_cast<uint8_t>(dist(rng)),
		static_cast<uint8_t>(dist(rng)),
		static_cast<uint8_t>(dist(rng)),
		255
	};
}
}

CircleModel::CircleModel()
{
	m_data.center = {200.0f, 200.0f};
	m_data.radius = 50.0f;
	m_data.thickness = 10.0f;
	m_data.thicknessColor = {255u, 255u, 255u, 255u};
	m_data.fillColor = {255u, 255u, 255u, 255u};
}

void CircleModel::SetCenter(const Point2f& center)
{
	m_data.center = center;
	NotifyObservers();
}

void CircleModel::ChangeRadius(float delta)
{
	if (m_data.radius + delta >= MIN_RADIUS)
	{
		m_data.radius += delta;
	}

	NotifyObservers();
}

void CircleModel::ChangeThickness(float delta)
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
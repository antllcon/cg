#include "CircleModel.h"

#include <stdexcept>

namespace
{
void AssertRadiusIsValid(float radius)
{
	if (radius < 10.0f || radius > 300.0f)
	{
		throw std::runtime_error("Радиус вышел за допустимые пределы (10-300)");
	}
}

} // namespace

CircleModel::CircleModel()
	: m_data({50.0f})
{
}

float CircleModel::GetRadius() const
{
	return m_data.radius;
}

void CircleModel::SetRadius(const float& radius)
{
	AssertRadiusIsValid(radius);
	if (m_data.radius != radius)
	{
		m_data.radius = radius;
		NotifyObservers();
	}
}

CircleData CircleModel::GetChangedData() const
{
	return CircleData{m_data.radius};
}

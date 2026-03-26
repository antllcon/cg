#include "LightModel.h"

LightModel::LightModel()
{
	m_data.color = Color::FromFloat(1.0f, 1.0f, 1.0f, 1.0f);
}

void LightModel::SetType(LightType type)
{
	m_data.type = type;
	NotifyObservers();
}

void LightModel::SetColor(const Color& color)
{
	m_data.color = color;
	NotifyObservers();
}

void LightModel::SetPosition(const Point3f& position)
{
	m_data.position = position;
	NotifyObservers();
}

void LightModel::SetDirection(const Point3f& direction)
{
	m_data.direction = direction;
	NotifyObservers();
}

void LightModel::SetIntensity(float intensity)
{
	m_data.intensity = intensity;
	NotifyObservers();
}

void LightModel::SetRange(float range)
{
	m_data.range = range;
	NotifyObservers();
}

const LightData& LightModel::GetData() const
{
	return m_data;
}

LightData LightModel::GetChangedData() const
{
	return m_data;
}
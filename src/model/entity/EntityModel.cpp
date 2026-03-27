#include "EntityModel.h"
#include "src/core/types/math/Math.h"

EntityModel::EntityModel()
{
	m_data.color = Color::FromRGBA(46, 204, 113, 255);
}

void EntityModel::SetPosition(const Point3f& position)
{
	m_data.transform.SetPosition(position);
	NotifyObservers();
}

void EntityModel::SetScale(const Point3f& scale)
{
	m_data.transform.SetScale(scale);
	NotifyObservers();
}

void EntityModel::Rotate(float angle, const Point3f& axis)
{
	Quat currentRotation = m_data.transform.GetRotation();
	Quat deltaRotation = Math::AngleAxis(angle, Math::Normalize(axis));

	m_data.transform.SetRotation(currentRotation * deltaRotation);
	NotifyObservers();
}

void EntityModel::SetColor(const Color& color)
{
	m_data.color = color;
	NotifyObservers();
}

const EntityData& EntityModel::GetData() const
{
	return m_data;
}

EntityData EntityModel::GetChangedData() const
{
	return m_data;
}
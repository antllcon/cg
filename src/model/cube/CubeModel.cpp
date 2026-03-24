#include "CubeModel.h"
#include <libs/glm/gtc/quaternion.hpp>

CubeModel::CubeModel()
{
	m_data.color = Color::FromRGBA(46, 204, 113, 255);
}

void CubeModel::Rotate(float angle, const Point3f& axis)
{
	glm::quat currentRotation = m_data.transform.GetRotation();
	glm::quat deltaRotation = glm::angleAxis(angle, glm::normalize(axis));

	m_data.transform.SetRotation(currentRotation * deltaRotation);
	NotifyObservers();
}

void CubeModel::SetColor(const Color& color)
{
	m_data.color = color;
	NotifyObservers();
}

const CubeData& CubeModel::GetData() const
{
	return m_data;
}

CubeData CubeModel::GetChangedData() const
{
	return m_data;
}
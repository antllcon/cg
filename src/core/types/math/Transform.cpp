#include "Transform.h"
#include <libs/glm/gtc/matrix_transform.hpp>

Transform::Transform()
	: m_position(0.0f, 0.0f, 0.0f)
	, m_rotation(1.0f, 0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
{
}

void Transform::SetPosition(const Point3f& position) noexcept
{
	m_position = position;
}

void Transform::SetRotation(const glm::quat& rotation) noexcept
{
	m_rotation = rotation;
}

void Transform::SetScale(const Point3f& scale) noexcept
{
	m_scale = scale;
}

Point3f Transform::GetPosition() const noexcept
{
	return m_position;
}

glm::quat Transform::GetRotation() const noexcept
{
	return m_rotation;
}

Point3f Transform::GetScale() const noexcept
{
	return m_scale;
}

glm::mat4 Transform::GetMatrix() const noexcept
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}
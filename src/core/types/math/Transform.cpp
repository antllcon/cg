#include "Transform.h"
#include "Math.h"

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

Mat4 Transform::GetMatrix() const noexcept
{
	Mat4 translationMatrix = Math::Translate(Mat4(1.0f), m_position);
	Mat4 rotationMatrix = Math::Mat4Cast(m_rotation);
	glm::mat4 scaleMatrix = Math::Scale(glm::mat4(1.0f), m_scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}
#include "CameraModel.h"
#include "src/core/types/math/Math.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
void AssertIsPositive(float value)
{
	if (value <= 0.0f)
	{
		throw std::runtime_error("Значение для камеры должно быть строго больше нуля");
	}
}
} // namespace

CameraModel::CameraModel()
	: m_yaw(-90.0f)
	, m_pitch(0.0f)
	, m_fov(68.0f)
	, m_aspect(1.8f)
	, m_near(0.1f)
	, m_far(100.0f)
	, m_worldUp(0.0f, 1.0f, 0.0f)
	, m_forward(0.0f, 0.0f, -1.0f)
	, m_right(1.0f, 0.0f, 0.0f)
	, m_up(0.0f, 1.0f, 0.0f)
{
	UpdateVectors();
}

void CameraModel::Init(const Point3f& position, float fov, float aspect, float nearPlane, float farPlane)
{
	AssertIsPositive(fov);
	AssertIsPositive(aspect);
	AssertIsPositive(nearPlane);
	AssertIsPositive(farPlane);

	m_data.position = position;
	m_fov = fov;
	m_aspect = aspect;
	m_near = nearPlane;
	m_far = farPlane;

	UpdateVectors();
}

void CameraModel::MoveBy(const Point3f& offset)
{
	m_data.position += offset;

	UpdateMatrices();
	NotifyObservers();
}

void CameraModel::Rotate(float yawOffset, float pitchOffset)
{
	m_yaw += yawOffset;
	m_pitch += pitchOffset;
	m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

	UpdateVectors();
	NotifyObservers();
}

void CameraModel::SetAspectRatio(float aspect)
{
	AssertIsPositive(aspect);
	m_aspect = aspect;

	UpdateMatrices();
	NotifyObservers();
}

Point3f CameraModel::GetForward() const noexcept
{
	return m_forward;
}

Point3f CameraModel::GetRight() const noexcept
{
	return m_right;
}

Point3f CameraModel::GetWorldUp() const noexcept
{
	return m_worldUp;
}

const CameraData& CameraModel::GetData() const
{
	return m_data;
}

CameraData CameraModel::GetChangedData() const
{
	return m_data;
}

void CameraModel::UpdateVectors()
{
	Point3f front;
	front.x = std::cos(Math::Radians(m_yaw)) * std::cos(Math::Radians(m_pitch));
	front.y = std::sin(Math::Radians(m_pitch));
	front.z = std::sin(Math::Radians(m_yaw)) * std::cos(Math::Radians(m_pitch));

	m_forward = Math::Normalize(front);
	m_right = Math::Normalize(Math::Cross(m_forward, m_worldUp));
	m_up = Math::Normalize(Math::Cross(m_right, m_forward));

	UpdateMatrices();
}

void CameraModel::UpdateMatrices()
{
	m_data.viewMatrix = Math::LookAt(m_data.position, m_data.position + m_forward, m_up);
	m_data.projectionMatrix = Math::Perspective(Math::Radians(m_fov), m_aspect, m_near, m_far);
}
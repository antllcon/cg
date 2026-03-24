#include "CameraModel.h"
#include <algorithm>
#include <cmath>
#include <libs/glm/gtc/matrix_transform.hpp>
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
	, m_fov(45.0f)
	, m_aspect(1.0f)
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

void CameraModel::Move(float forwardAmount, float rightAmount, float upAmount)
{
	m_data.position += m_forward * forwardAmount;
	m_data.position += m_right * rightAmount;
	m_data.position += m_worldUp * upAmount;

	UpdateMatrices();
	NotifyObservers();
}

void CameraModel::Rotate(float yawOffset, float pitchOffset)
{
	m_yaw += yawOffset;
	m_pitch += pitchOffset;

	m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

	UpdateVectors();
}

void CameraModel::SetAspectRatio(float aspect)
{
	AssertIsPositive(aspect);
	m_aspect = aspect;
	UpdateMatrices();
	NotifyObservers();
}

void CameraModel::SetFPS(uint32_t fps)
{
	m_data.fps = fps;
	NotifyObservers();
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
	front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	front.y = std::sin(glm::radians(m_pitch));
	front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

	m_forward = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));

	UpdateMatrices();
}

void CameraModel::UpdateMatrices()
{
	m_data.viewMatrix = glm::lookAt(m_data.position, m_data.position + m_forward, m_up);
	m_data.projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}
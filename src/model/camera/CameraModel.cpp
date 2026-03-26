#include "CameraModel.h"
#include <algorithm>
#include <cmath>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace
{
constexpr float BASE_VELOCITY = 5.0f;
constexpr float MOUSE_SENSITIVITY = 0.15f;
constexpr float SPEED_MULTIPLIER_STEP = 0.2f;
constexpr float MIN_SPEED_MULTIPLIER = 0.1f;
constexpr float MAX_SPEED_MULTIPLIER = 10.0f;

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
	, m_speedMultiplier(1.0f)
	, m_fpsTimer(0.0f)
	, m_fpsCount(0)
	, m_isForward(false)
	, m_isBackward(false)
	, m_isLeft(false)
	, m_isRight(false)
	, m_isUp(false)
	, m_isDown(false)
	, m_isMousePressed(false)
	, m_lastMousePos(0.0f, 0.0f)
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

void CameraModel::SetDirectionState(CameraDirection direction, bool isMoving)
{
	switch (direction)
	{
	case CameraDirection::Forward: m_isForward = isMoving; break;
	case CameraDirection::Backward: m_isBackward = isMoving; break;
	case CameraDirection::Left: m_isLeft = isMoving; break;
	case CameraDirection::Right: m_isRight = isMoving; break;
	case CameraDirection::Up: m_isUp = isMoving; break;
	case CameraDirection::Down: m_isDown = isMoving; break;
	}
}

void CameraModel::SetMousePressed(bool isPressed, const Point2f& position)
{
	m_isMousePressed = isPressed;
	if (isPressed)
	{
		m_lastMousePos = position;
	}
}

void CameraModel::ProcessMouseMovement(const Point2f& position)
{
	if (!m_isMousePressed)
	{
		return;
	}

	float xOffset = (position.x - m_lastMousePos.x) * MOUSE_SENSITIVITY;
	float yOffset = (m_lastMousePos.y - position.y) * MOUSE_SENSITIVITY;

	m_lastMousePos = position;

	m_yaw += xOffset;
	m_pitch += yOffset;
	m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

	UpdateVectors();
	NotifyObservers();
}

void CameraModel::AddSpeedMultiplier(float delta)
{
	m_speedMultiplier += delta * SPEED_MULTIPLIER_STEP;
	m_speedMultiplier = std::clamp(m_speedMultiplier, MIN_SPEED_MULTIPLIER, MAX_SPEED_MULTIPLIER);
}

void CameraModel::SetAspectRatio(float aspect)
{
	AssertIsPositive(aspect);
	m_aspect = aspect;
	UpdateMatrices();
	NotifyObservers();
}

void CameraModel::Update(float dt)
{
	bool isChanged = false;

	m_fpsTimer += dt;
	m_fpsCount++;

	if (m_fpsTimer >= 1.0f)
	{
		m_data.fps = m_fpsCount;
		m_fpsTimer -= 1.0f;
		m_fpsCount = 0;
		isChanged = true;
	}

	float velocity = BASE_VELOCITY * m_speedMultiplier * dt;
	float forwardAmount = 0.0f;
	float rightAmount = 0.0f;
	float upAmount = 0.0f;

	if (m_isForward) forwardAmount += velocity;
	if (m_isBackward) forwardAmount -= velocity;
	if (m_isRight) rightAmount += velocity;
	if (m_isLeft) rightAmount -= velocity;
	if (m_isUp) upAmount += velocity;
	if (m_isDown) upAmount -= velocity;

	if (forwardAmount != 0.0f || rightAmount != 0.0f || upAmount != 0.0f)
	{
		m_data.position += m_forward * forwardAmount;
		m_data.position += m_right * rightAmount;
		m_data.position += m_worldUp * upAmount;

		UpdateMatrices();
		isChanged = true;
	}

	if (isChanged)
	{
		NotifyObservers();
	}
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
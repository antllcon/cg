#include "CameraModel.h"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace
{
constexpr float MAX_PITCH = 89.0f;
constexpr float MIN_PITCH = -89.0f;
constexpr float FULL_CIRCLE = 360.0f;

constexpr Vector3 INIT_POSITION = {0.0f, 0.0f, 5.0f};

void AssertIsFinite(float value)
{
	if (std::isnan(value) || std::isinf(value))
	{
		throw std::invalid_argument("Значение должно быть конечным числом");
	}
}

float WrapYaw(float yaw)
{
	yaw = std::fmod(yaw, FULL_CIRCLE);
	if (yaw < 0.0f)
	{
		yaw += FULL_CIRCLE;
	}
	return yaw;
}

float ClampPitch(float pitch)
{
	return std::clamp(pitch, MIN_PITCH, MAX_PITCH);
}

Vector3 ApplyMovement(Vector3 position, float yaw, float dForward, float dRight, float dUp)
{
	float yawRad = yaw * (std::numbers::pi_v<float> / 180.0f);
	float forwardX = std::cos(yawRad);
	float forwardZ = std::sin(yawRad);
	float rightX = std::cos(yawRad - std::numbers::pi_v<float> / 2.0f);
	float rightZ = std::sin(yawRad - std::numbers::pi_v<float> / 2.0f);

	position.x += forwardX * dForward + rightX * dRight;
	position.y += dUp;
	position.z += forwardZ * dForward + rightZ * dRight;

	return position;
}
} // namespace

CameraModel::CameraModel()
{
	m_state.position = INIT_POSITION;
	m_state.yaw = CameraSettings::YAW_DEFAULT;
	m_state.pitch = CameraSettings::PITCH_DEFAULT;
	m_state.fov = CameraSettings::FOV_DEFAULT;
}

void CameraModel::MoveCamera(float dForward, float dRight, float dUp)
{
	AssertIsFinite(dForward);
	AssertIsFinite(dRight);
	AssertIsFinite(dUp);

	m_state.position = ApplyMovement(m_state.position, m_state.yaw, dForward, dRight, dUp);
}

void CameraModel::RotateCamera(float dYaw, float dPitch)
{
	AssertIsFinite(dYaw);
	AssertIsFinite(dPitch);

	m_state.yaw = WrapYaw(m_state.yaw + dYaw);
	m_state.pitch = ClampPitch(m_state.pitch + dPitch);
}

CameraState CameraModel::GetState() const
{
	return m_state;
}
#include "CameraModel.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
constexpr float MAX_PITCH = 89.0f;
constexpr float MIN_PITCH = -89.0f;
constexpr float FULL_CIRCLE = 360.0f;

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
} // namespace

CameraModel::CameraModel(const Vector3& startPosition)
{
	m_state.position = startPosition;
	m_state.yaw = CameraSettings::YAW_DEFAULT;
	m_state.pitch = CameraSettings::PITCH_DEFAULT;
	m_state.fov = CameraSettings::FOV_DEFAULT;
}

void CameraModel::SetPosition(const Vector3& position)
{
	AssertIsFinite(position.x);
	AssertIsFinite(position.y);
	AssertIsFinite(position.z);

	m_state.position = position;
	NotifyObservers();
}

void CameraModel::RotateCamera(float dYaw, float dPitch)
{
	AssertIsFinite(dYaw);
	AssertIsFinite(dPitch);

	m_state.yaw = WrapYaw(m_state.yaw + dYaw);
	m_state.pitch = ClampPitch(m_state.pitch + dPitch);
	NotifyObservers();
}

CameraState CameraModel::GetState() const
{
	return m_state;
}

CameraState CameraModel::GetChangedData() const
{
	return m_state;
}

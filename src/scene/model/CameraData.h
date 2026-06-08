#pragma once

#include "SceneData.h"

namespace CameraSettings
{
inline constexpr float YAW_DEFAULT = -90.0f;
inline constexpr float PITCH_DEFAULT = 0.0f;
inline constexpr float FOV_DEFAULT = 85.0f;
} // namespace CameraSettings

struct CameraState
{
	Vector3 position;
	float yaw = CameraSettings::YAW_DEFAULT;
	float pitch = CameraSettings::PITCH_DEFAULT;
	float fov = CameraSettings::FOV_DEFAULT;
};
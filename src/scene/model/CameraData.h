#pragma once

#include "SceneData.h"

namespace 
{
inline constexpr float YAW_DEFAULT = -90.0f;   
inline constexpr float PITCH_DEFAULT = 0.0f;   
inline constexpr float FOV_DEFAULT = 85.0f;   
}

struct CameraState
{
	Vector3 position;
	float yaw = YAW_DEFAULT;
	float pitch = PITCH_DEFAULT;
	float fov = FOV_DEFAULT;
};
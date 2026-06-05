#pragma once

#include "CameraData.h"

class CameraModel final
{
public:
	CameraModel();
	~CameraModel() = default;

	void MoveCamera(float dForward, float dRight, float dUp);
	void RotateCamera(float dYaw, float dPitch);

	CameraState GetState() const;

private:
	CameraState m_state;
};
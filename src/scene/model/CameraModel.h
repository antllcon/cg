#pragma once

#include "CameraData.h"
#include "stb_observer.h"

class CameraModel final : public CObservable<CameraState>
{
public:
	CameraModel();
	~CameraModel() override = default;

	void MoveCamera(float dForward, float dRight, float dUp);
	void RotateCamera(float dYaw, float dPitch);

	CameraState GetState() const;

protected:
	CameraState GetChangedData() const override;

private:
	CameraState m_state;
};
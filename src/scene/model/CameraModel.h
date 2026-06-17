#pragma once

#include "CameraData.h"
#include "stb_observer.h"

class CameraModel final : public CObservable<CameraState>
{
public:
	explicit CameraModel(const Vector3& startPosition);
	~CameraModel() override = default;

	void SetPosition(const Vector3& position);
	void RotateCamera(float dYaw, float dPitch);

	CameraState GetState() const;

protected:
	CameraState GetChangedData() const override;

private:
	CameraState m_state;
};

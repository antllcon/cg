#pragma once
#include "src/core/types/Point.h"
#include "src/core/types/scene/CameraData.h"
#include "src/system/Observer.h"

class CameraModel final : public CObservable<CameraData>
{
public:
	CameraModel();

	void Init(const Point3f& position, float fov, float aspect, float nearPlane, float farPlane);

	void Move(float forwardAmount, float rightAmount, float upAmount);
	void Rotate(float yawOffset, float pitchOffset);
	void SetAspectRatio(float aspect);
	void SetFPS(uint32_t fps);

	const CameraData& GetData() const;

protected:
	CameraData GetChangedData() const override;

private:
	void UpdateVectors();
	void UpdateMatrices();

	CameraData m_data;

	float m_yaw;
	float m_pitch;

	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;

	Point3f m_worldUp;
	Point3f m_forward;
	Point3f m_right;
	Point3f m_up;
};
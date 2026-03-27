#pragma once
#include "src/core/types/math/Point.h"
#include "src/core/types/scene/CameraData.h"
#include "src/system/Observer.h"

class CameraModel final : public CObservable<CameraData>
{
public:
	CameraModel();

	void Init(const Point3f& position, float fov, float aspect, float nearPlane, float farPlane);

	void MoveBy(const Point3f& offset);
	void Rotate(float yawOffset, float pitchOffset);
	void SetAspectRatio(float aspect);

	Point3f GetForward() const noexcept;
	Point3f GetRight() const noexcept;
	Point3f GetWorldUp() const noexcept;

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
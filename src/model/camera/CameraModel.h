#pragma once
#include "src/core/types/Point.h"
#include "src/core/types/scene/CameraData.h"
#include "src/system/Observer.h"

enum class CameraDirection
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

class CameraModel final : public CObservable<CameraData>
{
public:
	CameraModel();

	void Init(const Point3f& position, float fov, float aspect, float nearPlane, float farPlane);

	void SetDirectionState(CameraDirection direction, bool isMoving);
	void SetMousePressed(bool isPressed, const Point2f& position);
	void ProcessMouseMovement(const Point2f& position);
	void AddSpeedMultiplier(float delta);

	void SetAspectRatio(float aspect);
	void Update(float dt);

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

	float m_speedMultiplier;
	float m_fpsTimer;
	uint32_t m_fpsCount;

	bool m_isForward;
	bool m_isBackward;
	bool m_isLeft;
	bool m_isRight;
	bool m_isUp;
	bool m_isDown;

	bool m_isMousePressed;
	Point2f m_lastMousePos;

	Point3f m_worldUp;
	Point3f m_forward;
	Point3f m_right;
	Point3f m_up;
};
#pragma once
#include "Point.h"

class Transform final
{
public:
	Transform();

	void SetPosition(const Point3f& position) noexcept;
	void SetRotation(const Quat& rotation) noexcept;
	void SetScale(const Point3f& scale) noexcept;

	Point3f GetPosition() const noexcept;
	Quat GetRotation() const noexcept;
	Point3f GetScale() const noexcept;

	Mat4 GetMatrix() const noexcept;

private:
	Point3f m_position;
	Quat m_rotation;
	Point3f m_scale;
};
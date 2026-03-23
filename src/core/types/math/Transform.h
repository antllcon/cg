#pragma once
#include "src/core/types/Point.h"
#include <libs/glm/gtc/quaternion.hpp>

class Transform final
{
public:
	Transform();

	void SetPosition(const Point3f& position) noexcept;
	void SetRotation(const glm::quat& rotation) noexcept;
	void SetScale(const Point3f& scale) noexcept;

	Point3f GetPosition() const noexcept;
	glm::quat GetRotation() const noexcept;
	Point3f GetScale() const noexcept;

	glm::mat4 GetMatrix() const noexcept;

private:
	Point3f m_position;
	glm::quat m_rotation;
	Point3f m_scale;
};
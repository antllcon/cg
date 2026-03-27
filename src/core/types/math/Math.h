#pragma once
#include "Point.h"
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtc/type_ptr.hpp>

namespace Math
{
template <typename T>
auto Radians(T degrees)
{
	return glm::radians(degrees);
}

template <typename T>
auto Normalize(const T& value)
{
	return glm::normalize(value);
}

template <typename T, typename U>
auto Cross(const T& x, const U& y)
{
	return glm::cross(x, y);
}

inline Mat4 LookAt(const Point3f& eye, const Point3f& center, const Point3f& up)
{
	return glm::lookAt(eye, center, up);
}

template <typename T>
inline Mat4 Perspective(T fovy, T aspect, T zNear, T zFar)
{
	return glm::perspective(fovy, aspect, zNear, zFar);
}

template <typename T>
inline Quat AngleAxis(T angle, const Point3f& axis)
{
	return glm::angleAxis(angle, axis);
}

template <typename T>
auto ValuePtr(T& value)
{
	return glm::value_ptr(value);
}

template <typename T>
auto ValuePtr(const T& value)
{
	return glm::value_ptr(value);
}

template <typename T>
inline Mat4 Ortho(T left, T right, T bottom, T top, T zNear, T zFar)
{
	return glm::ortho(left, right, bottom, top, zNear, zFar);
}

template <typename T, typename U>
inline auto Translate(const T& matrix, const U& translation)
{
	return glm::translate(matrix, translation);
}

template <typename T, typename U>
inline auto Scale(const T& matrix, const U& scale)
{
	return glm::scale(matrix, scale);
}

template <typename T>
inline auto Mat4Cast(const T& quaternion)
{
	return glm::mat4_cast(quaternion);
}
}
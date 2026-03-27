#pragma once
#include "../types/math/Point.h"
#include "src/core/types/color/Color.h"
#include <string>

class IShader
{
public:
	virtual ~IShader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetInt(const std::string& name, int value) = 0;
	virtual void SetFloat(const std::string& name, float value) = 0;
	virtual void SetFloat2(const std::string& name, const Point2f& value) = 0;
	virtual void SetFloat3(const std::string& name, const Point3f& value) = 0;
	virtual void SetFloat4(const std::string& name, const Point4f& value) = 0;
	virtual void SetMat4(const std::string& name, const Mat4& value) = 0;
	virtual void SetColor(const std::string& name, const Color& color) = 0;
};
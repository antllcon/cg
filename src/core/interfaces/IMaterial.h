#pragma once
#include "src/core/interfaces/IShader.h"
#include "src/core/types/color/Color.h"
#include <memory>

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetShader(const std::shared_ptr<IShader>& shader) = 0;
	virtual std::shared_ptr<IShader> GetShader() const = 0;

	virtual void SetDiffuseColor(const Color& color) = 0;
	virtual void SetSpecularColor(const Color& color) = 0;
	virtual void SetShininess(float shininess) = 0;

	virtual Color GetDiffuseColor() const = 0;
	virtual Color GetSpecularColor() const = 0;
	virtual float GetShininess() const = 0;
};
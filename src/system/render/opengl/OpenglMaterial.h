#pragma once
#include "src/core/interfaces/IMaterial.h"

class OpenglMaterial final : public IMaterial
{
public:
	OpenglMaterial();
	~OpenglMaterial() override = default;

	void Bind() const override;
	void Unbind() const override;

	void SetShader(const std::shared_ptr<IShader>& shader) override;
	std::shared_ptr<IShader> GetShader() const override;

	void SetDiffuseColor(const Color& color) override;
	void SetSpecularColor(const Color& color) override;
	void SetShininess(float shininess) override;

	Color GetDiffuseColor() const override;
	Color GetSpecularColor() const override;
	float GetShininess() const override;

private:
	std::shared_ptr<IShader> m_shader;
	Color m_diffuseColor;
	Color m_specularColor;
	float m_shininess;
};
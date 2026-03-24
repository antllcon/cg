#include "OpenglMaterial.h"
#include <stdexcept>

namespace
{
void AssertIsShaderValid(const std::shared_ptr<IShader>& shader)
{
	if (!shader)
	{
		throw std::runtime_error("Шейдер не назначен материалу перед отрисовкой");
	}
}
} // namespace

OpenglMaterial::OpenglMaterial()
	: m_diffuseColor(Color::FromFloat(1.0f, 1.0f, 1.0f, 1.0f))
	, m_specularColor(Color::FromFloat(1.0f, 1.0f, 1.0f, 1.0f))
	, m_shininess(32.0f)
{
}

void OpenglMaterial::Bind() const
{
	AssertIsShaderValid(m_shader);

	m_shader->Bind();
	m_shader->SetColor("u_Material.diffuse", m_diffuseColor);
	m_shader->SetColor("u_Material.specular", m_specularColor);
	m_shader->SetFloat("u_Material.shininess", m_shininess);
}

void OpenglMaterial::Unbind() const
{
	if (m_shader)
	{
		m_shader->Unbind();
	}
}

void OpenglMaterial::SetShader(const std::shared_ptr<IShader>& shader)
{
	m_shader = shader;
}

std::shared_ptr<IShader> OpenglMaterial::GetShader() const
{
	return m_shader;
}

void OpenglMaterial::SetDiffuseColor(const Color& color)
{
	m_diffuseColor = color;
}

void OpenglMaterial::SetSpecularColor(const Color& color)
{
	m_specularColor = color;
}

void OpenglMaterial::SetShininess(float shininess)
{
	m_shininess = shininess;
}

Color OpenglMaterial::GetDiffuseColor() const
{
	return m_diffuseColor;
}

Color OpenglMaterial::GetSpecularColor() const
{
	return m_specularColor;
}

float OpenglMaterial::GetShininess() const
{
	return m_shininess;
}
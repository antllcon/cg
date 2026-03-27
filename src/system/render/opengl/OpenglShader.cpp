#include "OpenglShader.h"
#include "src/core/types/math/Math.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsShaderCompiled(uint32_t shader)
{
	int isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		glDeleteShader(shader);
		throw std::runtime_error("Ошибка компиляции шейдера");
	}
}

void AssertIsProgramLinked(uint32_t program)
{
	int isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		glDeleteProgram(program);
		throw std::runtime_error("Ошибка линковки шейдерной программы");
	}
}

uint32_t CompileShader(uint32_t type, const std::string& source)
{
	uint32_t shader = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	AssertIsShaderCompiled(shader);
	return shader;
}
} // namespace

OpenglShader::OpenglShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	m_rendererId = glCreateProgram();
	glAttachShader(m_rendererId, vertexShader);
	glAttachShader(m_rendererId, fragmentShader);
	glLinkProgram(m_rendererId);

	AssertIsProgramLinked(m_rendererId);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

OpenglShader::~OpenglShader()
{
	glDeleteProgram(m_rendererId);
}

void OpenglShader::Bind() const
{
	glUseProgram(m_rendererId);
}

void OpenglShader::Unbind() const
{
	glUseProgram(0);
}

void OpenglShader::SetInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void OpenglShader::SetFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void OpenglShader::SetFloat2(const std::string& name, const Point2f& value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenglShader::SetFloat3(const std::string& name, const Point3f& value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenglShader::SetFloat4(const std::string& name, const Point4f& value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenglShader::SetMat4(const std::string& name, const Mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, Math::ValuePtr(value));
}

void OpenglShader::SetColor(const std::string& name, const Color& color)
{
	glUniform4f(GetUniformLocation(name), color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

int OpenglShader::GetUniformLocation(const std::string& name) const
{
	if (m_uniformLocationCache.contains(name))
	{
		return m_uniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_rendererId, name.c_str());
	m_uniformLocationCache[name] = location;

	return location;
}
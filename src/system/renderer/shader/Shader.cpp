#include "Shader.h"
#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace
{
void AssertIsFileExists(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Файл шейдера не найден: " + path.string());
	}
}

void AssertIsFileOpen(const std::ifstream& file, const std::filesystem::path& path)
{
	if (!file.is_open())
	{
		throw std::runtime_error("Не удалось открыть файл шейдера: " + path.string());
	}
}

void AssertShaderCompiled(uint32_t shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::vector<char> infoLog(512);
		glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
		throw std::runtime_error(std::string("Ошибка компиляции шейдера: ") + infoLog.data());
	}
}

void AssertProgramLinked(uint32_t program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		std::vector<char> infoLog(512);
		glGetProgramInfoLog(program, 512, nullptr, infoLog.data());
		throw std::runtime_error(std::string("Ошибка сборки шейдерной программы: ") + infoLog.data());
	}
}

std::string ReadFileContent(const std::filesystem::path& path)
{
	AssertIsFileExists(path);

	std::ifstream file(path);
	AssertIsFileOpen(file, path);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

uint32_t CompileShader(uint32_t type, const std::string& source)
{
	uint32_t shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	AssertShaderCompiled(shader);

	return shader;
}
} // namespace

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	std::string vertexSource = ReadFileContent(vertexPath);
	std::string fragmentSource = ReadFileContent(fragmentPath);

	uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexShader);
	glAttachShader(m_programId, fragmentShader);
	glLinkProgram(m_programId);

	AssertProgramLinked(m_programId);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_programId);
}

void Shader::Use() const
{
	glUseProgram(m_programId);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value ? 1 : 0);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetFloat2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_programId, name.c_str()), x, y);
}

void Shader::SetMat4(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetFloat4(const std::string& name, const std::tuple<float, float, float, float>& value) const
{
	auto [x, y, z, w] = value;
	glUniform4f(glGetUniformLocation(m_programId, name.c_str()), x, y, z, w);
}
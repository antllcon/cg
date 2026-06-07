#pragma once

#include <filesystem>
#include <string>

class Shader final
{
public:
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void Use() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetFloat2(const std::string& name, float x, float y) const;
	void SetMat4(const std::string& name, const float* value) const;
	void SetFloat4(const std::string& name, const std::tuple<float, float, float, float>& value) const;

private:
	uint32_t m_programId = 0;
};
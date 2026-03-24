#pragma once
#include "src/core/interfaces/IShader.h"
#include <unordered_map>

class OpenglShader final : public IShader
{
public:
	OpenglShader(const std::string& vertexSource, const std::string& fragmentSource);
	~OpenglShader() override;

	void Bind() const override;
	void Unbind() const override;

	void SetInt(const std::string& name, int value) override;
	void SetFloat(const std::string& name, float value) override;
	void SetFloat2(const std::string& name, const Point2f& value) override;
	void SetFloat3(const std::string& name, const Point3f& value) override;
	void SetFloat4(const std::string& name, const Point4f& value) override;
	void SetMat4(const std::string& name, const glm::mat4& value) override;
	void SetColor(const std::string& name, const Color& color) override;

private:
	int GetUniformLocation(const std::string& name) const;

	uint32_t m_rendererId;
	mutable std::unordered_map<std::string, int> m_uniformLocationCache;
};
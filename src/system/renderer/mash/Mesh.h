#pragma once

#include <cstdint>
#include <vector>

class Mesh final
{
public:
	Mesh(const std::vector<float>& vertices, uint8_t componentsPerVertex);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	Mesh& operator=(Mesh&&) = delete;

	void Bind() const;
	static void Unbind();

	uint32_t GetVertexCount() const;

private:
	uint32_t m_vertexArrayObject = 0;
	uint32_t m_vertexBufferObject = 0;
	uint32_t m_vertexCount = 0;
};
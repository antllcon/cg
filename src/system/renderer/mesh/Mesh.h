#pragma once

#include <cstdint>
#include <span>

enum class PrimitiveType : uint8_t
{
	Triangles,
	LineStrip
};

class Mesh final
{
public:
	Mesh(std::span<const float> vertices, uint32_t componentsPerVertex, PrimitiveType primitive);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void Draw() const;

private:
	void Release() noexcept;

private:
	uint32_t m_vertexArrayObject = 0;
	uint32_t m_vertexBufferObject = 0;
	int32_t m_vertexCount = 0;
	PrimitiveType m_primitive = PrimitiveType::Triangles;
};
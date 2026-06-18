#include "Mesh.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
constexpr uint32_t POSITION_ATTRIBUTE_INDEX = 0u;

void AssertComponentsPerVertexIsValid(uint32_t componentsPerVertex)
{
	if (componentsPerVertex == 0u)
	{
		throw std::invalid_argument("Количество компонентов вершины должно быть положительным");
	}
}

int32_t CalculateVertexCount(std::span<const float> vertices, uint32_t componentsPerVertex)
{
	AssertComponentsPerVertexIsValid(componentsPerVertex);
	return static_cast<int32_t>(vertices.size() / componentsPerVertex);
}
} // namespace

Mesh::Mesh(std::span<const float> vertices, uint32_t componentsPerVertex)
	: m_vertexCount(CalculateVertexCount(vertices, componentsPerVertex))
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, static_cast<int>(componentsPerVertex), GL_FLOAT, GL_FALSE, static_cast<int>(componentsPerVertex * sizeof(float)), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	Release();
}

Mesh::Mesh(Mesh&& other) noexcept
	: m_vertexArrayObject(other.m_vertexArrayObject)
	, m_vertexBufferObject(other.m_vertexBufferObject)
	, m_vertexCount(other.m_vertexCount)
{
	other.m_vertexArrayObject = 0;
	other.m_vertexBufferObject = 0;
	other.m_vertexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		Release();

		m_vertexArrayObject = other.m_vertexArrayObject;
		m_vertexBufferObject = other.m_vertexBufferObject;
		m_vertexCount = other.m_vertexCount;

		other.m_vertexArrayObject = 0;
		other.m_vertexBufferObject = 0;
		other.m_vertexCount = 0;
	}
	return *this;
}

void Mesh::Draw() const
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindVertexArray(0);
}

void Mesh::Release() noexcept
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	glDeleteBuffers(1, &m_vertexBufferObject);

	m_vertexArrayObject = 0;
	m_vertexBufferObject = 0;
}
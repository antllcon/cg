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

GLenum ToGlPrimitive(PrimitiveType primitive)
{
	switch (primitive)
	{
	case PrimitiveType::Triangles: return GL_TRIANGLES;
	case PrimitiveType::LineStrip: return GL_LINE_STRIP;
	default: throw std::invalid_argument("Неизвестный тип примитива");
	}
}
} // namespace

Mesh::Mesh(std::span<const float> vertices, uint32_t componentsPerVertex, PrimitiveType primitive)
	: m_vertexCount(CalculateVertexCount(vertices, componentsPerVertex))
	, m_primitive(primitive)
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
	, m_primitive(other.m_primitive)
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
		m_primitive = other.m_primitive;

		other.m_vertexArrayObject = 0;
		other.m_vertexBufferObject = 0;
		other.m_vertexCount = 0;
	}
	return *this;
}

void Mesh::Draw() const
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(ToGlPrimitive(m_primitive), 0, m_vertexCount);
	glBindVertexArray(0);
}

void Mesh::Release() noexcept
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	glDeleteBuffers(1, &m_vertexBufferObject);

	m_vertexArrayObject = 0;
	m_vertexBufferObject = 0;
}
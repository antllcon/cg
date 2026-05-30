#include "Mesh.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsDataValid(const std::vector<float>& vertices, uint8_t components)
{
	if (vertices.empty())
	{
		throw std::invalid_argument("Массив вершин не должен быть пустым");
	}

	if (components == 0)
	{
		throw std::invalid_argument("Количество компонент на вершину должно быть больше нуля");
	}

	if (vertices.size() % components != 0)
	{
		throw std::invalid_argument("Размер массива вершин не кратен количеству компонент");
	}
}

void AssertIsGenerated(uint32_t id)
{
	if (id == 0)
	{
		throw std::runtime_error("Не удалось сгенерировать буфер или массив вершин OpenGL");
	}
}
} // namespace

Mesh::Mesh(const std::vector<float>& vertices, uint8_t componentsPerVertex)
{
	AssertIsDataValid(vertices, componentsPerVertex);

	m_vertexCount = static_cast<uint32_t>(vertices.size() / componentsPerVertex);

	glGenVertexArrays(1, &m_vertexArrayObject);
	AssertIsGenerated(m_vertexArrayObject);

	glGenBuffers(1, &m_vertexBufferObject);
	AssertIsGenerated(m_vertexBufferObject);

	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, componentsPerVertex, GL_FLOAT, GL_FALSE, componentsPerVertex * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	if (m_vertexBufferObject != 0)
	{
		glDeleteBuffers(1, &m_vertexBufferObject);
	}

	if (m_vertexArrayObject != 0)
	{
		glDeleteVertexArrays(1, &m_vertexArrayObject);
	}
}

void Mesh::Bind() const
{
	glBindVertexArray(m_vertexArrayObject);
}

void Mesh::Unbind()
{
	glBindVertexArray(0);
}

uint32_t Mesh::GetVertexCount() const
{
	return m_vertexCount;
}
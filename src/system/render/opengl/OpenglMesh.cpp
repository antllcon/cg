#include "OpenglMesh.h"
#include <stdexcept>

namespace
{
void AssertIsVertexArrayValid(const std::shared_ptr<IVertexArray>& vertexArray)
{
	if (!vertexArray)
	{
		throw std::runtime_error("Вершинный массив (VAO) не может быть null при создании меша");
	}
}

void AssertIsPositiveIndexCount(uint32_t indexCount)
{
	if (indexCount == 0)
	{
		throw std::runtime_error("Количество индексов меша должно быть больше нуля");
	}
}
} // namespace

OpenglMesh::OpenglMesh(std::shared_ptr<IVertexArray> vertexArray, uint32_t indexCount)
	: m_vertexArray(std::move(vertexArray))
	, m_indexCount(indexCount)
{
	AssertIsVertexArrayValid(m_vertexArray);
	AssertIsPositiveIndexCount(m_indexCount);
}

void OpenglMesh::Bind() const
{
	m_vertexArray->Bind();
}

void OpenglMesh::Unbind() const
{
	m_vertexArray->Unbind();
}

std::shared_ptr<IVertexArray> OpenglMesh::GetVertexArray() const
{
	return m_vertexArray;
}

uint32_t OpenglMesh::GetIndexCount() const
{
	return m_indexCount;
}
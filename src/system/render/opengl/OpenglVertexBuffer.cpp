#include "OpenglVertexBuffer.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsPositiveSize(uint32_t size)
{
	if (size == 0)
	{
		throw std::runtime_error("Размер вершинного буфера должен быть больше нуля");
	}
}
}

OpenglVertexBuffer::OpenglVertexBuffer(const void* vertices, uint32_t size)
	: m_rendererId(0)
{
	AssertIsPositiveSize(size);
	glGenBuffers(1, &m_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenglVertexBuffer::~OpenglVertexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void OpenglVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void OpenglVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenglVertexBuffer::SetLayout(const BufferLayout& layout)
{
	m_layout = layout;
}

const BufferLayout& OpenglVertexBuffer::GetLayout() const
{
	return m_layout;
}
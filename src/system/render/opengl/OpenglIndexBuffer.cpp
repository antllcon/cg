#include "OpenglIndexBuffer.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsPositiveCount(uint32_t count)
{
	if (count == 0)
	{
		throw std::runtime_error("Количество индексов должно быть больше нуля");
	}
}
}

OpenglIndexBuffer::OpenglIndexBuffer(const uint32_t* indices, uint32_t count)
	: m_rendererId(0)
	, m_count(count)
{
	AssertIsPositiveCount(count);
	glGenBuffers(1, &m_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenglIndexBuffer::~OpenglIndexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void OpenglIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void OpenglIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenglIndexBuffer::GetCount() const
{
	return m_count;
}
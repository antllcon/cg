#include "OpenglVertexArray.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsLayoutNotEmpty(const BufferLayout& layout)
{
	if (layout.GetElements().empty())
	{
		throw std::runtime_error("Вершинный буфер не имеет разметки (Layout)");
	}
}

uint32_t GetOpenglBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
	case ShaderDataType::Mat3:
	case ShaderDataType::Mat4:
		return GL_FLOAT;
	case ShaderDataType::Int:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4:
		return GL_INT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	case ShaderDataType::None:
		break;
	}

	throw std::runtime_error("Неизвестный тип данных шейдера");
}

uint32_t GetComponentCount(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:  return 1;
	case ShaderDataType::Float2: return 2;
	case ShaderDataType::Float3: return 3;
	case ShaderDataType::Float4: return 4;
	case ShaderDataType::Mat3:   return 3 * 3;
	case ShaderDataType::Mat4:   return 4 * 4;
	case ShaderDataType::Int:    return 1;
	case ShaderDataType::Int2:   return 2;
	case ShaderDataType::Int3:   return 3;
	case ShaderDataType::Int4:   return 4;
	case ShaderDataType::Bool:   return 1;
	case ShaderDataType::None:   break;
	}

	throw std::runtime_error("Неизвестный тип данных шейдера");
}
}

OpenglVertexArray::OpenglVertexArray()
	: m_rendererId(0)
	, m_vertexBufferIndex(0)
{
	glGenVertexArrays(1, &m_rendererId);
}

OpenglVertexArray::~OpenglVertexArray()
{
	glDeleteVertexArrays(1, &m_rendererId);
}

void OpenglVertexArray::Bind() const
{
	glBindVertexArray(m_rendererId);
}

void OpenglVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenglVertexArray::AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer)
{
	AssertIsLayoutNotEmpty(vertexBuffer->GetLayout());

	Bind();
	vertexBuffer->Bind();

	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout.GetElements())
	{
		glEnableVertexAttribArray(m_vertexBufferIndex);
		glVertexAttribPointer(
			m_vertexBufferIndex,
			GetComponentCount(element.type),
			GetOpenglBaseType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			reinterpret_cast<const void*>(element.offset));

		m_vertexBufferIndex++;
	}

	m_vertexBuffers.push_back(vertexBuffer);
}

void OpenglVertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer)
{
	Bind();
	indexBuffer->Bind();
	m_indexBuffer = indexBuffer;
}
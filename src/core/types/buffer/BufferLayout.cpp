#include "BufferLayout.h"
#include <stdexcept>

namespace
{
uint32_t GetShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float: return 4;
	case ShaderDataType::Float2: return 4 * 2;
	case ShaderDataType::Float3: return 4 * 3;
	case ShaderDataType::Float4: return 4 * 4;
	case ShaderDataType::Mat3: return 4 * 3 * 3;
	case ShaderDataType::Mat4: return 4 * 4 * 4;
	case ShaderDataType::Int: return 4;
	case ShaderDataType::Int2: return 4 * 2;
	case ShaderDataType::Int3: return 4 * 3;
	case ShaderDataType::Int4: return 4 * 4;
	case ShaderDataType::Bool: return 1;
	case ShaderDataType::None: break;
	}

	throw std::runtime_error("Неизвестный тип данных шейдера");
}
} // namespace

BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
	: name(name)
	, type(type)
	, size(GetShaderDataTypeSize(type))
	, offset(0)
	, normalized(normalized)
{
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
	: m_elements(elements)
{
	CalculateOffsetsAndStride();
}

uint32_t BufferLayout::GetStride() const noexcept
{
	return m_stride;
}

const std::vector<BufferElement>& BufferLayout::GetElements() const noexcept
{
	return m_elements;
}

void BufferLayout::CalculateOffsetsAndStride()
{
	size_t offset = 0;
	m_stride = 0;

	for (auto& element : m_elements)
	{
		element.offset = offset;
		offset += element.size;
		m_stride += element.size;
	}
}
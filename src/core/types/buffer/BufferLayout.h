#pragma once
#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

enum class ShaderDataType
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

struct BufferElement
{
	std::string name;
	ShaderDataType type;
	uint32_t size;
	size_t offset;
	bool normalized;

	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);
};

class BufferLayout final
{
public:
	BufferLayout() = default;
	BufferLayout(const std::initializer_list<BufferElement>& elements);

	uint32_t GetStride() const noexcept;
	const std::vector<BufferElement>& GetElements() const noexcept;

private:
	void CalculateOffsetsAndStride();

	std::vector<BufferElement> m_elements;
	uint32_t m_stride = 0;
};
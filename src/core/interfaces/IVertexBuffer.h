#pragma once
#include "src/core/types/buffer/BufferLayout.h"

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetLayout(const BufferLayout& layout) = 0;
	virtual const BufferLayout& GetLayout() const = 0;
};
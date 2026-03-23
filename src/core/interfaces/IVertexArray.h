#pragma once
#include "src/core/interfaces/IIndexBuffer.h"
#include "src/core/interfaces/IVertexBuffer.h"
#include <memory>

class IVertexArray
{
public:
	virtual ~IVertexArray() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) = 0;
	virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) = 0;
};
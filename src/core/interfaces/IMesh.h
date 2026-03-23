#pragma once
#include "src/core/interfaces/IVertexArray.h"
#include <cstdint>
#include <memory>

class IMesh
{
public:
	virtual ~IMesh() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual std::shared_ptr<IVertexArray> GetVertexArray() const = 0;
	virtual uint32_t GetIndexCount() const = 0;
};
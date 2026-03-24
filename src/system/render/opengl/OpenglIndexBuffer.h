#pragma once
#include "src/core/interfaces/IIndexBuffer.h"

class OpenglIndexBuffer final : public IIndexBuffer
{
public:
	OpenglIndexBuffer(const uint32_t* indices, uint32_t count);
	~OpenglIndexBuffer() override;

	void Bind() const override;
	void Unbind() const override;

	uint32_t GetCount() const override;

private:
	uint32_t m_rendererId;
	uint32_t m_count;
};
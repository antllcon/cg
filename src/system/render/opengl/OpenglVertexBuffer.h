#pragma once
#include "src/core/interfaces/IVertexBuffer.h"

class OpenglVertexBuffer final : public IVertexBuffer
{
public:
	OpenglVertexBuffer(const void* vertices, uint32_t size);
	~OpenglVertexBuffer() override;

	void Bind() const override;
	void Unbind() const override;

	void SetLayout(const BufferLayout& layout) override;
	const BufferLayout& GetLayout() const override;

private:
	uint32_t m_rendererId;
	BufferLayout m_layout;
};
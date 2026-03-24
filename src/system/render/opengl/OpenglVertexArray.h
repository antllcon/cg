#pragma once
#include "src/core/interfaces/IVertexArray.h"
#include <vector>

class OpenglVertexArray final : public IVertexArray
{
public:
	OpenglVertexArray();
	~OpenglVertexArray() override;

	void Bind() const override;
	void Unbind() const override;

	void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) override;
	void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) override;

private:
	uint32_t m_rendererId;
	uint32_t m_vertexBufferIndex;
	std::vector<std::shared_ptr<IVertexBuffer>> m_vertexBuffers;
	std::shared_ptr<IIndexBuffer> m_indexBuffer;
};
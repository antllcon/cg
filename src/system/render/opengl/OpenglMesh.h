#pragma once
#include "src/core/interfaces/IMesh.h"
#include <memory>

class OpenglMesh final : public IMesh
{
public:
	OpenglMesh(std::shared_ptr<IVertexArray> vertexArray, uint32_t indexCount);
	~OpenglMesh() override = default;

	void Bind() const override;
	void Unbind() const override;

	std::shared_ptr<IVertexArray> GetVertexArray() const override;
	uint32_t GetIndexCount() const override;

private:
	std::shared_ptr<IVertexArray> m_vertexArray;
	uint32_t m_indexCount;
};
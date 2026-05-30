#pragma once

#include <cstdint>

class CanvasMesh final
{
public:
	CanvasMesh();
	~CanvasMesh();

	CanvasMesh(const CanvasMesh&) = delete;
	CanvasMesh& operator=(const CanvasMesh&) = delete;
	CanvasMesh(CanvasMesh&& other) = delete;
	CanvasMesh& operator=(CanvasMesh&& other) = delete;

	void Bind() const;
	void Unbind() const;
	void Draw() const;

private:
	void Free() noexcept;

	uint32_t m_vertexArrayObject = 0;
	uint32_t m_vertexBufferObject = 0;
};
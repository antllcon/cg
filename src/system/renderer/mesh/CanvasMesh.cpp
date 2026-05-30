#include "CanvasMesh.h"
#include <array>
#include <glad/glad.h>

namespace
{
constexpr int32_t VERTEX_COUNT = 6;
constexpr int32_t COMPONENTS_PER_VERTEX = 4;
constexpr uint8_t VERTICES_SIZE = VERTEX_COUNT * COMPONENTS_PER_VERTEX;
constexpr std::array<float, VERTICES_SIZE> VERTICES = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f};

constexpr uint32_t POSITION_LOCATION = 0u;
constexpr int32_t POSITION_COMPONENTS = 2;
constexpr void* POSITION_OFFSET = nullptr;

constexpr uint32_t TEXTURE_LOCATION = 1u;
constexpr int32_t TEXTURE_COMPONENTS = 2;
const auto TEXTURE_OFFSET = reinterpret_cast<void*>(POSITION_COMPONENTS * sizeof(float));
constexpr int32_t STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
} // namespace

CanvasMesh::CanvasMesh()
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

	glBufferData(
	   GL_ARRAY_BUFFER,
	   VERTICES.size() * sizeof(float),
	   VERTICES.data(),
	   GL_STATIC_DRAW);

	glVertexAttribPointer(
		   POSITION_LOCATION,
		   POSITION_COMPONENTS,
		   GL_FLOAT,
		   GL_FALSE,
		   STRIDE,
		   POSITION_OFFSET);
	glEnableVertexAttribArray(POSITION_LOCATION);

	glVertexAttribPointer(
		TEXTURE_LOCATION,
		TEXTURE_COMPONENTS,
		GL_FLOAT,
		GL_FALSE,
		STRIDE,
		TEXTURE_OFFSET);
	glEnableVertexAttribArray(TEXTURE_LOCATION);

	glBindVertexArray(0);
}

CanvasMesh::~CanvasMesh()
{
	Free();
}

void CanvasMesh::Bind() const
{
	glBindVertexArray(m_vertexArrayObject);
}

void CanvasMesh::Unbind() const
{
	glBindVertexArray(0);
}

void CanvasMesh::Draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
}

void CanvasMesh::Free() noexcept
{
	if (m_vertexArrayObject != 0)
	{
		glDeleteVertexArrays(1, &m_vertexArrayObject);
		m_vertexArrayObject = 0;
	}

	if (m_vertexBufferObject != 0)
	{
		glDeleteBuffers(1, &m_vertexBufferObject);
		m_vertexBufferObject = 0;
	}
}
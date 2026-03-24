#include "OpenglGeometryFactory.h"

#include "src/core/types/Point.h"
#include "src/system/render/opengl/OpenglIndexBuffer.h"
#include "src/system/render/opengl/OpenglMesh.h"
#include "src/system/render/opengl/OpenglVertexArray.h"
#include "src/system/render/opengl/OpenglVertexBuffer.h"
#include <stdexcept>

namespace
{
struct Vertex3D
{
	Point3f position;
	Point3f normal;
	Point2f texCoords;
};

void AssertIsPositiveSize(float size)
{
	if (size <= 0.0f)
	{
		throw std::runtime_error("Размер генерируемой геометрии должен быть больше нуля");
	}
}

std::shared_ptr<IMesh> BuildMesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices)
{
	auto vertexBuffer = std::make_shared<OpenglVertexBuffer>(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Vertex3D)));

	BufferLayout layout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aNormal"},
		{ShaderDataType::Float2, "aTexCoord"}
	};
	vertexBuffer->SetLayout(layout);

	auto indexBuffer = std::make_shared<OpenglIndexBuffer>(indices.data(), static_cast<uint32_t>(indices.size()));

	auto vertexArray = std::make_shared<OpenglVertexArray>();
	vertexArray->AddVertexBuffer(vertexBuffer);
	vertexArray->SetIndexBuffer(indexBuffer);

	return std::make_shared<OpenglMesh>(vertexArray, indexBuffer->GetCount());
}

void PushCubeFace(
	std::vector<Vertex3D>& vertices,
	std::vector<uint32_t>& indices,
	const Point3f& center,
	const Point3f& right,
	const Point3f& up,
	const Point3f& normal,
	float halfSize)
{
	uint32_t startIndex = static_cast<uint32_t>(vertices.size());

	vertices.push_back({center - right * halfSize - up * halfSize, normal, {0.0f, 0.0f}});
	vertices.push_back({center + right * halfSize - up * halfSize, normal, {1.0f, 0.0f}});
	vertices.push_back({center + right * halfSize + up * halfSize, normal, {1.0f, 1.0f}});
	vertices.push_back({center - right * halfSize + up * halfSize, normal, {0.0f, 1.0f}});

	indices.push_back(startIndex);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 3);
	indices.push_back(startIndex);
}
}

std::shared_ptr<IMesh> OpenglGeometryFactory::CreateCube(float size)
{
	AssertIsPositiveSize(size);

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;
	float halfSize = size / 2.0f;

	PushCubeFace(vertices, indices, {0.0f, 0.0f, halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, halfSize);
	PushCubeFace(vertices, indices, {0.0f, 0.0f, -halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, halfSize);
	PushCubeFace(vertices, indices, {-halfSize, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, halfSize);
	PushCubeFace(vertices, indices, {halfSize, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, halfSize);
	PushCubeFace(vertices, indices, {0.0f, halfSize, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, halfSize);
	PushCubeFace(vertices, indices, {0.0f, -halfSize, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, halfSize);

	return BuildMesh(vertices, indices);
}

std::shared_ptr<IMesh> OpenglGeometryFactory::CreatePlane(float width, float depth)
{
	AssertIsPositiveSize(width);
	AssertIsPositiveSize(depth);

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;
	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;

	PushCubeFace(vertices, indices, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, 1.0f);

	vertices[0].position = {-halfWidth, 0.0f, halfDepth};
	vertices[1].position = {halfWidth, 0.0f, halfDepth};
	vertices[2].position = {halfWidth, 0.0f, -halfDepth};
	vertices[3].position = {-halfWidth, 0.0f, -halfDepth};

	return BuildMesh(vertices, indices);
}

std::shared_ptr<IMesh> OpenglGeometryFactory::CreateSphere(float, uint32_t, uint32_t)
{
	throw std::runtime_error("Генерация сферы еще не реализована");
}

std::shared_ptr<IMesh> OpenglGeometryFactory::CreateCylinder(float, float, float, uint32_t)
{
	throw std::runtime_error("Генерация цилиндра еще не реализована");
}
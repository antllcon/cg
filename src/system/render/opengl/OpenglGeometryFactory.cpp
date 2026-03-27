#include "OpenglGeometryFactory.h"

#include "../../../core/types/math/Point.h"
#include "src/system/render/opengl/OpenglIndexBuffer.h"
#include "src/system/render/opengl/OpenglMesh.h"
#include "src/system/render/opengl/OpenglVertexArray.h"
#include "src/system/render/opengl/OpenglVertexBuffer.h"

#include <fstream>
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

void AssertIsModelExists(bool exists)
{
	if (!exists)
	{
		throw std::runtime_error("Файл модели .obj не найден по указанному пути");
	}
}

void AssertIsModelOpened(bool opened)
{
	if (!opened)
	{
		throw std::runtime_error("Не удалось открыть файл модели .obj для чтения");
	}
}

std::vector<std::string> SplitString(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}

Vertex3D ParseFaceVertex(
	const std::string& vertexData,
	const std::vector<Point3f>& positions,
	const std::vector<Point2f>& texCoords,
	const std::vector<Point3f>& normals)
{
	auto tokens = SplitString(vertexData, '/');
	Vertex3D vertex{};

	if (!tokens.empty() && !tokens[0].empty())
	{
		vertex.position = positions[std::stoul(tokens[0]) - 1];
	}

	if (tokens.size() > 1 && !tokens[1].empty())
	{
		vertex.texCoords = texCoords[std::stoul(tokens[1]) - 1];
	}

	if (tokens.size() > 2 && !tokens[2].empty())
	{
		vertex.normal = normals[std::stoul(tokens[2]) - 1];
	}

	return vertex;
}

std::shared_ptr<IMesh> BuildMesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices)
{
	auto vertexBuffer = std::make_shared<OpenglVertexBuffer>(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Vertex3D)));

	BufferLayout layout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aNormal"},
		{ShaderDataType::Float2, "aTexCoord"}};
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
} // namespace

std::shared_ptr<IMesh> OpenglGeometryFactory::CreateCube(float size)
{
	AssertIsPositiveSize(size);

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;
	float halfSize = size / 2.0f;

	PushCubeFace(vertices, indices, {0.0f, 0.0f, halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, halfSize);
	PushCubeFace(vertices, indices, {0.0f, 0.0f, -halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, halfSize);
	PushCubeFace(vertices, indices, {-halfSize, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, halfSize);
	PushCubeFace(vertices, indices, {halfSize, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, halfSize);
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

std::shared_ptr<IMesh> OpenglGeometryFactory::CreateFromObj(const std::filesystem::path& path)
{
	AssertIsModelExists(std::filesystem::exists(path));

	std::ifstream file(path);
	AssertIsModelOpened(file.is_open());

	std::vector<Point3f> tempPositions;
	std::vector<Point2f> tempTexCoords;
	std::vector<Point3f> tempNormals;

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;
	uint32_t currentIndex = 0;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v")
		{
			Point3f pos;
			iss >> pos.x >> pos.y >> pos.z;
			tempPositions.push_back(pos);
		}
		else if (prefix == "vt")
		{
			Point2f tex;
			iss >> tex.x >> tex.y;
			tempTexCoords.push_back(tex);
		}
		else if (prefix == "vn")
		{
			Point3f norm;
			iss >> norm.x >> norm.y >> norm.z;
			tempNormals.push_back(norm);
		}
		else if (prefix == "f")
		{
			std::string vertexStr;
			std::vector<uint32_t> faceIndices;

			while (iss >> vertexStr)
			{
				vertices.push_back(ParseFaceVertex(vertexStr, tempPositions, tempTexCoords, tempNormals));
				faceIndices.push_back(currentIndex++);
			}

			for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
			{
				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[i]);
				indices.push_back(faceIndices[i + 1]);
			}
		}
	}

	return BuildMesh(vertices, indices);
}
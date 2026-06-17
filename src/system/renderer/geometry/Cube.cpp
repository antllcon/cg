#include "Cube.h"
#include <array>

namespace Geometry
{
namespace
{
constexpr float HALF = 0.5f;

struct Corner
{
	float x;
	float y;
	float z;
};

struct Face
{
	Corner a;
	Corner b;
	Corner c;
	Corner d;
	float norX;
	float norY;
	float norZ;
};

constexpr std::array<Face, 6> FACES = {{
	{{HALF, -HALF, HALF}, {HALF, -HALF, -HALF}, {HALF, HALF, -HALF}, {HALF, HALF, HALF}, 1.0f, 0.0f, 0.0f},
	{{-HALF, -HALF, -HALF}, {-HALF, -HALF, HALF}, {-HALF, HALF, HALF}, {-HALF, HALF, -HALF}, -1.0f, 0.0f, 0.0f},
	{{-HALF, HALF, HALF}, {HALF, HALF, HALF}, {HALF, HALF, -HALF}, {-HALF, HALF, -HALF}, 0.0f, 1.0f, 0.0f},
	{{-HALF, -HALF, -HALF}, {HALF, -HALF, -HALF}, {HALF, -HALF, HALF}, {-HALF, -HALF, HALF}, 0.0f, -1.0f, 0.0f},
	{{-HALF, -HALF, HALF}, {HALF, -HALF, HALF}, {HALF, HALF, HALF}, {-HALF, HALF, HALF}, 0.0f, 0.0f, 1.0f},
	{{HALF, -HALF, -HALF}, {-HALF, -HALF, -HALF}, {-HALF, HALF, -HALF}, {HALF, HALF, -HALF}, 0.0f, 0.0f, -1.0f}}};

Vertex MakeVertex(const Corner& corner, const Face& face)
{
	return {corner.x, corner.y, corner.z, face.norX, face.norY, face.norZ};
}

void AppendFace(std::vector<Vertex>& vertices, const Face& face)
{
	vertices.push_back(MakeVertex(face.a, face));
	vertices.push_back(MakeVertex(face.b, face));
	vertices.push_back(MakeVertex(face.c, face));

	vertices.push_back(MakeVertex(face.a, face));
	vertices.push_back(MakeVertex(face.c, face));
	vertices.push_back(MakeVertex(face.d, face));
}
} // namespace

std::vector<Vertex> BuildUnitCube()
{
	std::vector<Vertex> vertices;
	vertices.reserve(FACES.size() * 6);

	for (const auto& face : FACES)
	{
		AppendFace(vertices, face);
	}

	return vertices;
}
} // namespace Geometry

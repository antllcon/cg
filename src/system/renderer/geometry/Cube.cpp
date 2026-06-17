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

Vertex MakeVertex(const Corner& corner, const Face& face, float u, float v)
{
	return {corner.x, corner.y, corner.z, face.norX, face.norY, face.norZ, u, v};
}

void AppendFace(std::vector<Vertex>& vertices, const Face& face)
{
	const Vertex a = MakeVertex(face.a, face, 0.0f, 0.0f);
	const Vertex b = MakeVertex(face.b, face, 1.0f, 0.0f);
	const Vertex c = MakeVertex(face.c, face, 1.0f, 1.0f);
	const Vertex d = MakeVertex(face.d, face, 0.0f, 1.0f);

	vertices.push_back(a);
	vertices.push_back(b);
	vertices.push_back(c);

	vertices.push_back(a);
	vertices.push_back(c);
	vertices.push_back(d);
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

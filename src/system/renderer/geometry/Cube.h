#pragma once

#include <vector>

namespace Geometry
{
struct Vertex
{
	float posX;
	float posY;
	float posZ;
	float norX;
	float norY;
	float norZ;
};

std::vector<Vertex> BuildUnitCube();
} // namespace Geometry

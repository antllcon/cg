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
	float texU;
	float texV;
};

std::vector<Vertex> BuildUnitCube();
} // namespace Geometry

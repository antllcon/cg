#pragma once

#include <cstdint>
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

std::vector<Vertex> BuildMobiusStrip(
	uint32_t segmentsA,
	uint32_t segmentsB,
	float radius,
	float halfWidth);
} // namespace Geometry
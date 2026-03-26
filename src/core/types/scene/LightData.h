#pragma once

enum class LightType
{
	Directional,
	Point,
	Spot
};

struct LightData
{
	LightType type{LightType::Directional};
	Color color;
	Point3f position{0.0f, 0.0f, 0.0f};
	Point3f direction{0.0f, -1.0f, 0.0f};
	float intensity{1.0f};
	float range{10.0f};
};
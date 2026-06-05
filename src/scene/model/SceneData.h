#pragma once

#include "src/utils/color/Color.h"

struct Vector3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

enum class ObjectType
{
	Cylinder,
	Torus
};

struct SceneObject
{
	ObjectType type;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Color color;
	bool isEmissive;
};
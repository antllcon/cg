#pragma once

#include "src/system/renderer/RenderableShape.h"
#include "src/utils/color/Color.h"

struct Vector3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct SceneObject
{
	RenderableShape shape = RenderableShape::MobiusStrip;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale = {1.0f, 1.0f, 1.0f};
	Color color;

	float specularStrength = 0.5f;
	float shininess = 32.0f;
};
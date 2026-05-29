#pragma once

#include "src/utils/color/Color.h"
#include <vector>

struct Vec3
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
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
	Color color;
	bool isEmissive;
};

struct CameraState
{
	Vec3 position;
	float yaw;
	float pitch;
	float fov;
};

struct ModelData
{
	CameraState camera;
	std::vector<SceneObject> objects;
};
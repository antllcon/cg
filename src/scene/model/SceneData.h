#pragma once

#include "src/system/renderer/RenderableShape.h"
#include "src/system/renderer/texture/TextureKey.h"
#include "src/utils/color/Color.h"

struct Vector3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct SceneObject
{
	RenderableShape shape = RenderableShape::Cube;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale = {1.0f, 1.0f, 1.0f};
	Color color = Color::FromRGBA(255, 255, 255);

	TextureKey texture = TextureKey::Wall1;
	float uvScaleU = 1.0f;
	float uvScaleV = 1.0f;
	bool useShadow = false;

	float specularStrength = 0.2f;
	float shininess = 16.0f;
};
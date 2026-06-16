#pragma once

#include "SceneData.h"
#include "src/utils/color/Color.h"

struct Light
{
	Vector3 position;
	Color color;

	float ambientStrength = 0.12f;

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};
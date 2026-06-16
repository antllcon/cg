#pragma once

#include "CameraData.h"
#include "LightData.h"
#include "SceneData.h"
#include <vector>

struct RenderData
{
	CameraState camera;
	std::vector<SceneObject> objects;
	Light light;
};
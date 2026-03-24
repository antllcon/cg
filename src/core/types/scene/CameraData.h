#pragma once
#include "src/core/types/Point.h"
#include <libs/glm/mat4x4.hpp>

struct CameraData
{
	glm::mat4 viewMatrix{1.0f};
	glm::mat4 projectionMatrix{1.0f};
	Point3f position{0.0f, 0.0f, 0.0f};
	uint32_t fps{0};
};
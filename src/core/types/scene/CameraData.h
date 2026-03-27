#pragma once

struct CameraData
{
	Mat4 viewMatrix{1.0f};
	Mat4 projectionMatrix{1.0f};
	Point3f position{2.0f, 0.0f, 2.0f};
};

#pragma once

#include "src/scene/model/SceneData.h"

class TextureLibrary;

class SkySphere final
{
public:
	SkySphere();
	~SkySphere();

	SkySphere(const SkySphere&) = delete;
	SkySphere& operator=(const SkySphere&) = delete;
	SkySphere(SkySphere&&) = delete;
	SkySphere& operator=(SkySphere&&) = delete;

	void Render(const TextureLibrary& textures, const Vector3& cameraPosition) const;

private:
	void* m_quadric;
};

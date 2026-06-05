#pragma once

#include "SceneData.h"
#include <vector>

class SceneModel final
{
public:
	SceneModel();
	~SceneModel() = default;

	void Update(float dt);

	std::vector<SceneObject> GetObjects() const;

private:
	std::vector<SceneObject> m_objects;
	float m_totalTime;
};
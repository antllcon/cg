#pragma once

#include "LightData.h"
#include "SceneData.h"
#include "stb_observer.h"
#include <vector>

class SceneModel final : public CObservable<std::vector<SceneObject>>
{
public:
	SceneModel();
	~SceneModel() override = default;

	void Update(float dt);

	std::vector<SceneObject> GetObjects() const;
	Light GetLight() const;

protected:
	std::vector<SceneObject> GetChangedData() const override;

private:
	std::vector<SceneObject> m_objects;
	Light m_light;
	float m_totalTime;
};
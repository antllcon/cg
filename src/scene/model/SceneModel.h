#pragma once

#include "LightData.h"
#include "SceneData.h"
#include "src/scene/maze/Maze.h"
#include "stb_observer.h"
#include <memory>
#include <vector>

class SceneModel final : public CObservable<std::vector<SceneObject>>
{
public:
	explicit SceneModel(std::shared_ptr<const Maze> maze);
	~SceneModel() override = default;

	void Update(float dt);

	std::vector<SceneObject> GetObjects() const;
	Light GetLight() const;

protected:
	std::vector<SceneObject> GetChangedData() const override;

private:
	std::shared_ptr<const Maze> m_maze;
	std::vector<SceneObject> m_objects;
	Light m_light;
};

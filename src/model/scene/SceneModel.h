#pragma once
#include "src/model/entity/EntityModel.h"
#include "src/model/light/LightModel.h"
#include <memory>
#include <vector>

class SceneModel
{
public:
	virtual ~SceneModel() = default;

	void AddEntity(std::shared_ptr<EntityModel> entity);
	void AddLight(std::shared_ptr<LightModel> light);

	const std::vector<std::shared_ptr<EntityModel>>& GetEntities() const noexcept;
	const std::vector<std::shared_ptr<LightModel>>& GetLights() const noexcept;

private:
	std::vector<std::shared_ptr<EntityModel>> m_entities;
	std::vector<std::shared_ptr<LightModel>> m_lights;
};
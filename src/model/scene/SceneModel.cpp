#include "SceneModel.h"
#include <stdexcept>

namespace
{
void AssertIsNotNull(const void* ptr)
{
	if (!ptr)
	{
		throw std::runtime_error("Указатель на модель не может быть пустым");
	}
}
}

void SceneModel::AddEntity(std::shared_ptr<EntityModel> entity)
{
	AssertIsNotNull(entity.get());
	m_entities.push_back(std::move(entity));
}

void SceneModel::AddLight(std::shared_ptr<LightModel> light)
{
	AssertIsNotNull(light.get());
	m_lights.push_back(std::move(light));
}

const std::vector<std::shared_ptr<EntityModel>>& SceneModel::GetEntities() const noexcept
{
	return m_entities;
}

const std::vector<std::shared_ptr<LightModel>>& SceneModel::GetLights() const noexcept
{
	return m_lights;
}
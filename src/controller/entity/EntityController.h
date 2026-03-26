#pragma once
#include "src/controller/IController.h"
#include "src/model/entity/EntityModel.h"
#include <memory>

class EntityController final : public IController
{
public:
	explicit EntityController(std::shared_ptr<EntityModel> model);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<EntityModel> m_model;
};
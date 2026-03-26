#include "EntityController.h"

EntityController::EntityController(std::shared_ptr<EntityModel> model)
	: m_model(std::move(model))
{
}

void EntityController::Update(float)
{
}

void EntityController::HandleEvent(const Event&)
{
}
#include "LettersController.h"

LettersController::LettersController(std::shared_ptr<LettersModel> model)
	: m_model(std::move(model))
{
}

void LettersController::Update(float dt)
{
	(void)dt;
}

#include "SunController.h"

SunController::SunController(std::shared_ptr<SunModel> model)
	: m_model(std::move(model))
{
}

void SunController::Update(float dt)
{
	m_model->Update(dt);
}

void SunController::HandleEvent(const Event&)
{
}

void SunController::SetTimeScale(float timeScale)
{
	m_model->SetTimeScale(timeScale);
}

void SunController::SetTimeOfDay(float timeOfDay)
{
	m_model->SetTimeOfDay(timeOfDay);
}
#include "ToastController.h"
#include <stdexcept>

namespace
{
constexpr float TOAST_DURATION_SECONDS = 3.0f;

void AssertIsNotEmpty(const std::string& text)
{
	if (text.empty())
	{
		throw std::runtime_error("Текст уведомления не может быть пустым");
	}
}
} // namespace

ToastController::ToastController(std::shared_ptr<ToastModel> model)
	: m_model(std::move(model))
{
}

void ToastController::Update(float dt)
{
	m_model->UpdateState(dt);
}

void ToastController::HandleEvent(const Event&)
{
}

void ToastController::ShowMessage(const std::string& message)
{
	AssertIsNotEmpty(message);
	m_model->Show(message, TOAST_DURATION_SECONDS);
}

void ToastController::ShowError(const std::string& errorMessage)
{
	AssertIsNotEmpty(errorMessage);
	m_model->Show(errorMessage, TOAST_DURATION_SECONDS);
}
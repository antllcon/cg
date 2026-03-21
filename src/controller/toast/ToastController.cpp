#include "ToastController.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
constexpr float TOAST_DURATION_SECONDS = 3.0f;
constexpr float ANIMATION_SPEED = 10.0f;
constexpr float MAX_OFFSET_Y = 60.0f;
constexpr float MIN_VISIBLE_ALPHA = 0.01f;

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
	, m_timer(0.0f)
{
}

void ToastController::Update(float dt)
{
	const ToastData& data = m_model->GetData();

	if (!data.isVisible && data.alpha <= MIN_VISIBLE_ALPHA)
	{
		return;
	}

	if (m_timer > 0.0f)
	{
		m_timer -= dt;
	}

	float targetAlpha = m_timer > 0.0f ? 1.0f : 0.0f;
	float targetOffset = m_timer > 0.0f ? MAX_OFFSET_Y : 0.0f;
	float t = std::min(ANIMATION_SPEED * dt, 1.0f);

	float newAlpha = std::lerp(data.alpha, targetAlpha, t);
	float newOffset = std::lerp(data.offsetY, targetOffset, t);

	m_model->UpdateAnimation(newAlpha, newOffset);

	if (m_timer <= 0.0f && newAlpha <= MIN_VISIBLE_ALPHA)
	{
		m_model->Hide();
	}
}

void ToastController::HandleEvent(const Event&)
{
}

void ToastController::ShowMessage(const std::string& message)
{
	AssertIsNotEmpty(message);
	m_timer = TOAST_DURATION_SECONDS;
	m_model->Show(message);
}

void ToastController::ShowError(const std::string& errorMessage)
{
	AssertIsNotEmpty(errorMessage);
	m_timer = TOAST_DURATION_SECONDS;
	m_model->Show(errorMessage);
}
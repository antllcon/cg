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
	, m_currentAlpha(0.0f)
	, m_currentOffset(0.0f)
{
}

void ToastController::Update(float dt)
{
	if (!m_model->GetData().isVisible && m_currentAlpha <= MIN_VISIBLE_ALPHA)
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

	m_currentAlpha = std::lerp(m_currentAlpha, targetAlpha, t);
	m_currentOffset = std::lerp(m_currentOffset, targetOffset, t);

	m_model->UpdateAnimation(m_currentAlpha, m_currentOffset);

	if (m_timer <= 0.0f && m_currentAlpha <= MIN_VISIBLE_ALPHA)
	{
		m_currentAlpha = 0.0f;
		m_currentOffset = 0.0f;
		m_model->Hide();
	}
}

void ToastController::HandleEvent(const Event&)
{
}

void ToastController::ShowError(const std::string& errorMessage)
{
	AssertIsNotEmpty(errorMessage);
	m_timer = TOAST_DURATION_SECONDS;
	m_model->Show(errorMessage);
}
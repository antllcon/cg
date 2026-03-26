#include "ToastModel.h"
#include <algorithm>
#include <cmath>

namespace
{
constexpr float ANIMATION_SPEED = 10.0f;
constexpr float MAX_OFFSET_Y = 60.0f;
constexpr float MIN_VISIBLE_ALPHA = 0.01f;
} // namespace

void ToastModel::Show(const std::string& message, float durationSeconds)
{
	m_data.message = message;
	m_data.isVisible = true;
	m_data.timeLeft = durationSeconds;
	NotifyObservers();
}

void ToastModel::Hide()
{
	if (m_data.isVisible)
	{
		m_data.isVisible = false;
		m_data.timeLeft = 0.0f;
		NotifyObservers();
	}
}

void ToastModel::UpdateState(float dt)
{
	if (!m_data.isVisible && m_data.alpha <= MIN_VISIBLE_ALPHA)
	{
		return;
	}

	if (m_data.timeLeft > 0.0f)
	{
		m_data.timeLeft -= dt;
	}

	float targetAlpha = m_data.timeLeft > 0.0f ? 1.0f : 0.0f;
	float targetOffset = m_data.timeLeft > 0.0f ? MAX_OFFSET_Y : 0.0f;
	float t = std::min(ANIMATION_SPEED * dt, 1.0f);

	m_data.alpha = std::lerp(m_data.alpha, targetAlpha, t);
	m_data.offsetY = std::lerp(m_data.offsetY, targetOffset, t);

	if (m_data.timeLeft <= 0.0f && m_data.alpha <= MIN_VISIBLE_ALPHA)
	{
		m_data.isVisible = false;
	}

	NotifyObservers();
}

const ToastData& ToastModel::GetData() const
{
	return m_data;
}

ToastData ToastModel::GetChangedData() const
{
	return m_data;
}
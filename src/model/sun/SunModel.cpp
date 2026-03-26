#include "SunModel.h"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace
{
constexpr float HOURS_IN_DAY = 24.0f;
constexpr float SUNRISE_HOUR = 6.0f;
constexpr float DAY_DURATION = 12.0f;

void AssertIsValidTime(float timeHours)
{
	if (timeHours < 0.0f || timeHours >= HOURS_IN_DAY)
	{
		throw std::runtime_error("Время суток должно быть в диапазоне от 0.0 до 24.0");
	}
}

Color LerpColor(const Color& a, const Color& b, float t)
{
	float r = std::lerp(a.GetR(), b.GetR(), t);
	float g = std::lerp(a.GetG(), b.GetG(), t);
	float b_channel = std::lerp(a.GetB(), b.GetB(), t);
	return Color::FromFloat(r, g, b_channel, 1.0f);
}
}

SunModel::SunModel()
	: m_timeOfDay(12.0f)
	, m_timeScale(1.0f)
{
	m_data.type = LightType::Directional;
	RecalculateSun();
}

void SunModel::Update(float dt)
{
	m_timeOfDay += dt * m_timeScale;

	if (m_timeOfDay >= HOURS_IN_DAY)
	{
		m_timeOfDay -= HOURS_IN_DAY;
	}

	RecalculateSun();
	NotifyObservers();
}

void SunModel::SetTimeScale(float scale)
{
	m_timeScale = scale;
	NotifyObservers();
}

void SunModel::SetTimeOfDay(float timeHours)
{
	AssertIsValidTime(timeHours);
	m_timeOfDay = timeHours;
	RecalculateSun();
	NotifyObservers();
}

float SunModel::GetTimeOfDay() const noexcept
{
	return m_timeOfDay;
}

float SunModel::GetTimeScale() const noexcept
{
	return m_timeScale;
}

void SunModel::RecalculateSun()
{
	float angle = (m_timeOfDay / HOURS_IN_DAY) * 2.0f * std::numbers::pi_v<float>;

	Point3f sunDir;
	sunDir.x = -std::cos(angle);
	sunDir.y = -std::sin(angle);
	sunDir.z = -0.3f;

	m_data.direction = sunDir;

	float sunHeight = std::sin((m_timeOfDay - SUNRISE_HOUR) / DAY_DURATION * std::numbers::pi_v<float>);
	float intensity = std::clamp(sunHeight, 0.1f, 1.2f);
	m_data.intensity = intensity;

	Color nightColor = Color::FromFloat(0.1f, 0.15f, 0.3f, 1.0f);
	Color dayColor = Color::FromFloat(1.0f, 0.95f, 0.9f, 1.0f);

	float colorBlend = std::clamp(sunHeight * 2.0f, 0.0f, 1.0f);
	m_data.color = LerpColor(nightColor, dayColor, colorBlend);
}
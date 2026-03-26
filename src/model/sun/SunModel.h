#pragma once
#include "src/model/light/LightModel.h"

class SunModel final : public LightModel
{
public:
	SunModel();

	void Update(float dt);
	void SetTimeScale(float scale);
	void SetTimeOfDay(float timeHours);

	float GetTimeOfDay() const noexcept;
	float GetTimeScale() const noexcept;

private:
	void RecalculateSun();

	float m_timeOfDay;
	float m_timeScale;
};
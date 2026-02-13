#include "LettersController.h"
#include "src/model/LettersModel.h"

namespace
{
constexpr float GRAVITY = 2000.0f;
constexpr float JUMP_SPEED = 600.0f;
constexpr float PHASE_DELAY = 0.3f;
constexpr float REST_TIME = 0.1f;

float CalculateJumpDisplacement(float time)
{
	return (JUMP_SPEED * time) - (0.5f * GRAVITY * time * time);
}
} // namespace

LettersController::LettersController(std::shared_ptr<LettersModel> model)
	: m_model(std::move(model))
{
	InitTimers();
}

void LettersController::Update(float dt)
{
	auto& data = m_model->GetData();
	bool isChanged = false;

	for (size_t i = 0; i < data.size(); ++i)
	{
		if (UpdateLetterState(i, dt))
		{
			isChanged = true;
		}
	}

	if (isChanged)
	{
		m_model->NotifyObservers();
	}
}

bool LettersController::UpdateLetterState(size_t index, float dt)
{
	m_letterTimers[index] += dt;
	const float time = m_letterTimers[index];
	auto& letter = m_model->GetData()[index];

	if (time < 0.0f)
	{
		letter.position = letter.basePosition;
		return false;
	}

	const float displacement = CalculateJumpDisplacement(time);
	const float newY = letter.basePosition.y - displacement;

	if (newY >= letter.basePosition.y)
	{
		letter.position = letter.basePosition;
		m_letterTimers[index] = -REST_TIME;
	}
	else
	{
		letter.position.y = newY;
	}

	return true;
}

void LettersController::InitTimers()
{
	const size_t count = m_model->GetData().size();
	m_letterTimers.resize(count);

	for (size_t i = 0; i < count; ++i)
	{
		m_letterTimers[i] = -(static_cast<float>(i) * PHASE_DELAY);
	}
}
#include "LettersController.h"
#include "src/model/LettersModel.h"

namespace
{
constexpr auto GRAVITY = 1000.0f;
constexpr auto JUMP_SPEED = 600.0f;
constexpr auto PHASE_DELAY = 0.4f;

float CalculateJumpDisplacement(float time)
{
	return JUMP_SPEED * time - 0.5f * GRAVITY * time * time;
}
} // namespace

LettersController::LettersController(std::shared_ptr<LettersModel> model)
	: m_model(std::move(model))
{
	InitTimers();
}

void LettersController::Update(float dt)
{
	m_model->BeginUpdate();

	for (size_t i = 0; i < m_model->GetData().size(); ++i)
	{
		UpdateLetterState(i, dt);
	}

	m_model->EndUpdate();
}

void LettersController::UpdateLetterState(size_t index, float dt)
{
	m_letterTimers[index] += dt;
	const auto time = m_letterTimers[index];

	if (time < 0.0f)
	{
		return;
	}

	const auto letter = m_model->GetData()[index];
	const auto displacement = CalculateJumpDisplacement(time);
	const sf::Vector2f newPosition = {letter.basePosition.x, letter.basePosition.y - displacement};

	if (newPosition.y <= letter.basePosition.y)
	{
		m_model->SetLetterPosition(index, newPosition);
	}
	else
	{
		m_model->SetLetterPosition(index, letter.basePosition);
		m_letterTimers[index] = 0.0f;
	}
}

void LettersController::InitTimers()
{
	const size_t countLetters = m_model->GetData().size();
	m_letterTimers.resize(countLetters);

	for (size_t i = 0; i < countLetters; ++i)
	{
		m_letterTimers[i] = -(static_cast<float>(i) * PHASE_DELAY);
	}
}
#include "KeyboardController.h"
#include "src/model/game/GameTypes.h"
#include "src/model/game/KeyboardModel.h"
#include "src/model/game/ScoreModel.h"
#include "src/model/game/WordModel.h"
#include <stdexcept>

namespace
{
void AssertIsModelValid(const void* model)
{
	if (model == nullptr)
	{
		throw std::runtime_error("Указатель на модель не может быть пустым");
	}
}
} // namespace

KeyboardController::KeyboardController(
	std::shared_ptr<KeyboardModel> keyboardModel,
	std::shared_ptr<WordModel> wordModel,
	std::shared_ptr<ScoreModel> scoreModel,
	std::vector<Riddle> riddles)
	: m_keyboardModel(keyboardModel)
	, m_wordModel(wordModel)
	, m_scoreModel(scoreModel)
	, m_riddles(riddles)
{
	AssertIsModelValid(m_keyboardModel.get());
	AssertIsModelValid(m_wordModel.get());
	AssertIsModelValid(m_scoreModel.get());

	if (m_riddles.empty())
	{
		throw std::runtime_error("Список загадок не может быть пустым");
	}
}

void KeyboardController::Update(float)
{
}

void KeyboardController::OnLetterClicked(char letter)
{
	if (m_keyboardModel->GetLetterStatus(letter) != LetterStatus::AVAILABLE)
	{
		return;
	}

	bool isHit = m_wordModel->OpenLetter(letter);

	if (isHit)
	{
		m_keyboardModel->SetLetterStatus(letter, LetterStatus::GUESSED_CORRECTLY);

		if (m_wordModel->IsFullyGuessed())
		{
			m_scoreModel->AddWin();
			LoadNextRiddle();
		}
	}
	else
	{
		m_keyboardModel->SetLetterStatus(letter, LetterStatus::GUESSED_INCORRECTLY);

		if (m_scoreModel->AddMistakeAndCheckLoss())
		{
			m_scoreModel->Reset();
			LoadNextRiddle();
		}
	}
}

void KeyboardController::StartGame()
{
	m_scoreModel->Reset();
	m_currentRiddleIndex = 0;
	LoadNextRiddle();
}

void KeyboardController::LoadNextRiddle()
{
	m_keyboardModel->Reset();
	m_wordModel->SetNewRiddle(m_riddles[m_currentRiddleIndex]);

	m_currentRiddleIndex = (m_currentRiddleIndex + 1) % m_riddles.size();
}
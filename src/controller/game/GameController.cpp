#include "GameController.h"
#include "KeyboardController.h"
#include "ScoreController.h"
#include "WordController.h"
#include <stdexcept>
#include <random>

namespace
{
void AssertIsControllerValid(const void* controller)
{
	if (controller == nullptr)
	{
		throw std::runtime_error("Указатель на контроллер не может быть пустым");
	}
}

void AssertIsRiddlesValid(const std::vector<Riddle>& riddles)
{
	if (riddles.empty())
	{
		throw std::runtime_error("Список загадок не может быть пустым");
	}
}
}

GameController::GameController(
	std::shared_ptr<KeyboardController> keyboardController,
	std::shared_ptr<WordController> wordController,
	std::shared_ptr<ScoreController> scoreController,
	std::vector<Riddle> riddles)
	: m_keyboardController(keyboardController)
	, m_wordController(wordController)
	, m_scoreController(scoreController)
	, m_riddles(std::move(riddles))
{
	AssertIsControllerValid(m_keyboardController.get());
	AssertIsControllerValid(m_wordController.get());
	AssertIsControllerValid(m_scoreController.get());
	AssertIsRiddlesValid(m_riddles);

	std::random_device rd;
	std::mt19937 g(rd());
	std::ranges::shuffle(m_riddles, g);
}

void GameController::Update(float)
{
}

void GameController::StartGame()
{
	m_scoreController->Reset();
	m_currentRiddleIndex = 0;
	LoadNextRiddle();
}

void GameController::ProcessLetter(char letter)
{
	if (m_keyboardController->GetLetterStatus(letter) != LetterStatus::AVAILABLE)
	{
		return;
	}

	bool isHit = m_wordController->OpenLetter(letter);

	if (isHit)
	{
		m_keyboardController->SetLetterStatus(letter, LetterStatus::GUESSED_CORRECTLY);

		if (m_wordController->IsFullyGuessed())
		{
			m_scoreController->AddWin();
			LoadNextRiddle();
		}
	}
	else
	{
		m_keyboardController->SetLetterStatus(letter, LetterStatus::GUESSED_INCORRECTLY);

		if (m_scoreController->AddMistakeAndCheckLoss())
		{
			m_scoreController->Reset();
			LoadNextRiddle();
		}
	}
}

void GameController::LoadNextRiddle()
{
	m_keyboardController->Reset();
	m_wordController->SetNewRiddle(m_riddles[m_currentRiddleIndex]);

	m_currentRiddleIndex = (m_currentRiddleIndex + 1) % m_riddles.size();
}
#pragma once
#include "../IController.h"
#include "src/model/game/GameTypes.h"
#include <memory>
#include <vector>

class KeyboardController;
class WordController;
class ScoreController;

class GameController final : public IController
{
public:
	GameController(
		std::shared_ptr<KeyboardController> keyboardController,
		std::shared_ptr<WordController> wordController,
		std::shared_ptr<ScoreController> scoreController,
		std::vector<Riddle> riddles);

	void Update(float dt) override;
	void StartGame();
	void ProcessLetter(char letter);

private:
	std::shared_ptr<KeyboardController> m_keyboardController;
	std::shared_ptr<WordController> m_wordController;
	std::shared_ptr<ScoreController> m_scoreController;

	std::vector<Riddle> m_riddles;
	size_t m_currentRiddleIndex = 0;

	void LoadNextRiddle();
};
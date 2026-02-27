#pragma once
#include "../IController.h"
#include "src/model/game/GameTypes.h"
#include <memory>
#include <vector>

class KeyboardModel;
class WordModel;
class ScoreModel;

class KeyboardController final : public IController
{
public:
	KeyboardController(
		std::shared_ptr<KeyboardModel> keyboardModel,
		std::shared_ptr<WordModel> wordModel,
		std::shared_ptr<ScoreModel> scoreModel,
		std::vector<Riddle> riddles);
	void Update(float dt) override;
	void OnLetterClicked(char letter);
	void StartGame();

private:
	std::shared_ptr<KeyboardModel> m_keyboardModel;
	std::shared_ptr<WordModel> m_wordModel;
	std::shared_ptr<ScoreModel> m_scoreModel;

	std::vector<Riddle> m_riddles;
	size_t m_currentRiddleIndex = 0;

	void LoadNextRiddle();
};
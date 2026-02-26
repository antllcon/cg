#pragma once
#include "../IController.h"
#include <memory>

class KeyboardModel;
class WordModel;

class KeyboardController final : public IController
{
public:
	KeyboardController(std::shared_ptr<KeyboardModel> keyboardModel, std::shared_ptr<WordModel> wordModel);

	void Update(float dt) override;
	void OnLetterClicked(char letter);

private:
	std::shared_ptr<KeyboardModel> m_keyboardModel;
	std::shared_ptr<WordModel> m_wordModel;
};
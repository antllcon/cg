#pragma once
#include "../IController.h"
#include "src/model/game/GameTypes.h"
#include <functional>
#include <memory>

class KeyboardModel;

class KeyboardController final : public IController
{
public:
	explicit KeyboardController(std::shared_ptr<KeyboardModel> keyboardModel);

	void Update(float dt) override;
	void OnLetterClicked(char letter);
	void SetLetterCallback(std::function<void(char)> callback);

	void SetLetterStatus(char letter, LetterStatus status);
	LetterStatus GetLetterStatus(char letter) const;
	void Reset();

private:
	std::shared_ptr<KeyboardModel> m_keyboardModel;
	std::function<void(char)> m_onLetterClicked;
};
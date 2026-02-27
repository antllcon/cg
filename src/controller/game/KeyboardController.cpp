#include "KeyboardController.h"
#include "src/model/game/KeyboardModel.h"
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
}

KeyboardController::KeyboardController(std::shared_ptr<KeyboardModel> keyboardModel)
	: m_keyboardModel(keyboardModel)
{
	AssertIsModelValid(m_keyboardModel.get());
}

void KeyboardController::Update(float)
{
}

void KeyboardController::OnLetterClicked(char letter)
{
	if (m_onLetterClicked)
	{
		m_onLetterClicked(letter);
	}
}

void KeyboardController::SetLetterCallback(std::function<void(char)> callback)
{
	m_onLetterClicked = std::move(callback);
}

void KeyboardController::SetLetterStatus(char letter, LetterStatus status)
{
	m_keyboardModel->SetLetterStatus(letter, status);
}

LetterStatus KeyboardController::GetLetterStatus(char letter) const
{
	return m_keyboardModel->GetLetterStatus(letter);
}

void KeyboardController::Reset()
{
	m_keyboardModel->Reset();
}
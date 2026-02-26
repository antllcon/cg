#include "KeyboardController.h"
#include "src/model/game/GameTypes.h"
#include "src/model/game/KeyboardModel.h"
#include "src/model/game/WordModel.h"
#include <cctype>
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

void AssertIsLetterValid(char letter)
{
	if (!std::isalpha(static_cast<unsigned char>(letter)))
	{
		throw std::runtime_error("Символ должен быть латинской буквой");
	}
}
}

KeyboardController::KeyboardController(std::shared_ptr<KeyboardModel> keyboardModel, std::shared_ptr<WordModel> wordModel)
	: m_keyboardModel(keyboardModel)
	, m_wordModel(wordModel)
{
	AssertIsModelValid(m_keyboardModel.get());
	AssertIsModelValid(m_wordModel.get());
}

void KeyboardController::Update(float)
{
}

void KeyboardController::OnLetterClicked(char letter)
{
	AssertIsLetterValid(letter);

	if (m_keyboardModel->GetLetterStatus(letter) != LetterStatus::AVAILABLE)
	{
		return;
	}

	bool isHit = m_wordModel->OpenLetter(letter);

	if (isHit)
	{
		m_keyboardModel->SetLetterStatus(letter, LetterStatus::GUESSED_CORRECTLY);
	}
	else
	{
		m_keyboardModel->SetLetterStatus(letter, LetterStatus::GUESSED_INCORRECTLY);
	}
}
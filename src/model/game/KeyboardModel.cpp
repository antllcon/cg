#include "KeyboardModel.h"
#include <stdexcept>
#include <cctype>

namespace
{
void AssertIsLetterValid(char letter)
{
	if (!std::isalpha(static_cast<unsigned char>(letter)))
	{
		throw std::runtime_error("Символ клавиатуры должен быть латинской буквой");
	}
}

char ToUpperCase(char letter)
{
	return static_cast<char>(std::toupper(static_cast<unsigned char>(letter)));
}
}

KeyboardModel::KeyboardModel()
{
	InitializeKeys();
}

void KeyboardModel::Reset()
{
	InitializeKeys();
	NotifyObservers();
}

void KeyboardModel::SetLetterStatus(char letter, LetterStatus status)
{
	AssertIsLetterValid(letter);
	char upperLetter = ToUpperCase(letter);

	if (m_data.keys[upperLetter] != status)
	{
		m_data.keys[upperLetter] = status;
		NotifyObservers();
	}
}

LetterStatus KeyboardModel::GetLetterStatus(char letter) const
{
	AssertIsLetterValid(letter);
	char upperLetter = ToUpperCase(letter);

	auto iterator = m_data.keys.find(upperLetter);
	if (iterator != m_data.keys.end())
	{
		return iterator->second;
	}

	return LetterStatus::AVAILABLE;
}

KeyboardData KeyboardModel::GetChangedData() const
{
	return m_data;
}

void KeyboardModel::InitializeKeys()
{
	m_data.keys.clear();
	for (char character = 'A'; character <= 'Z'; ++character)
	{
		m_data.keys[character] = LetterStatus::AVAILABLE;
	}
}
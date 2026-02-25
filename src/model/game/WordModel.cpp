#include "WordModel.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace
{
    void AssertIsRiddleValid(const Riddle& riddle)
    {
        if (riddle.word.empty())
        {
            throw std::runtime_error("Слово для загадки не может быть пустым");
        }
    }

    char ToUpperCase(char letter)
    {
        return static_cast<char>(std::toupper(static_cast<unsigned char>(letter)));
    }
}

WordModel::WordModel()
{
    m_data.isHintRevealed = false;
}

void WordModel::SetNewRiddle(const Riddle& riddle)
{
    AssertIsRiddleValid(riddle);

    m_data.riddle = riddle;
    m_data.isHintRevealed = false;

    std::ranges::transform(m_data.riddle.word,
        m_data.riddle.word.begin(),
        ToUpperCase
    );

    UpdateMask();
    NotifyObservers();
}

bool WordModel::OpenLetter(char letter)
{
    bool isHit = false;
    char upperLetter = ToUpperCase(letter);

    for (size_t index = 0; index < m_data.riddle.word.length(); ++index)
    {
        if (m_data.riddle.word[index] == upperLetter)
        {
            m_data.maskedWord[index] = upperLetter;
            isHit = true;
        }
    }

    if (isHit)
    {
        NotifyObservers();
    }

    return isHit;
}

void WordModel::RevealHint()
{
    if (!m_data.isHintRevealed)
    {
        m_data.isHintRevealed = true;
        NotifyObservers();
    }
}

const std::string& WordModel::GetMaskedWord() const
{
    return m_data.maskedWord;
}

const std::string& WordModel::GetDescription() const
{
    return m_data.riddle.description;
}

const std::string& WordModel::GetHint() const
{
    return m_data.riddle.hint;
}

bool WordModel::IsHintRevealed() const
{
    return m_data.isHintRevealed;
}

bool WordModel::IsFullyGuessed() const
{
    if (m_data.maskedWord.empty())
    {
        return false;
    }

    return m_data.maskedWord.find('_') == std::string::npos;
}

WordData WordModel::GetChangedData() const
{
    return m_data;
}

void WordModel::UpdateMask()
{
    m_data.maskedWord = std::string(m_data.riddle.word.length(), '_');
}
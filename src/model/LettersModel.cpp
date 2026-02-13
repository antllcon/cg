#include "LettersModel.h"

LettersModel::LettersModel()
{
	// m_data.push_back({LetterType::G, {100.0f, 200.0f}, sf::Color::Cyan});
	m_data.push_back({LetterType::S, {200.0f, 200.0f}, sf::Color::Magenta});
	m_data.push_back({LetterType::V, {300.0f, 200.0f}, sf::Color::Blue});
	m_data.push_back({LetterType::O, {400.0f, 200.0f}, sf::Color::Red});
}

LettersData& LettersModel::GetData()
{
	return m_data;
}

const LettersData& LettersModel::GetData() const
{
	return m_data;
}

LettersData LettersModel::GetChangedData() const
{
	return m_data;
}
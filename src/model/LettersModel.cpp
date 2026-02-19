#include "LettersModel.h"

#include <stdexcept>

namespace
{
void AssertIsValidIndex(size_t index, size_t size)
{
	if (index >= size)
	{
		throw std::out_of_range("Индекс буквы выходит за пределы допустимого диапазона");
	}
}
}

LettersModel::LettersModel()
	: m_isUpdating(false)
{
	m_data.push_back({LetterType::G, {160.0f, 320.0f}, {160.0f, 320.0f}, sf::Color(100, 160, 100)});
	m_data.push_back({LetterType::S, {280.0f, 320.0f}, {280.0f, 320.0f}, sf::Color(100, 156, 160)});
	m_data.push_back({LetterType::V, {400.0f, 320.0f}, {400.0f, 320.0f}, sf::Color(104, 100, 160)});
	// m_data.push_back({LetterType::O, {520.0f, 320.0f}, {520.0f, 320.0f}, sf::Color(160, 100, 132)});
}

void LettersModel::SetLetterPosition(size_t index, const sf::Vector2f& position)
{
	AssertIsValidIndex(index, m_data.size());

	m_data[index].position = position;

	if (!m_isUpdating)
	{
		NotifyObservers();
	}
}

const LettersData& LettersModel::GetData() const
{
	return m_data;
}

void LettersModel::BeginUpdate()
{
	m_isUpdating = true;
}

void LettersModel::EndUpdate()
{
	m_isUpdating = false;
	NotifyObservers();
}

LettersData LettersModel::GetChangedData() const
{
	return m_data;
}
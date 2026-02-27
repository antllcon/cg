#include "ScoreModel.h"

ScoreModel::ScoreModel()
{
	m_data.score = 0;
	m_data.mistakes = 0;
	m_data.maxMistakes = 6;
}

void ScoreModel::AddWin()
{
	m_data.score++;
	m_data.mistakes = 0;
	NotifyObservers();
}

bool ScoreModel::AddMistakeAndCheckLoss()
{
	m_data.mistakes++;
	NotifyObservers();
	return m_data.mistakes >= m_data.maxMistakes;
}

void ScoreModel::Reset()
{
	m_data.score = 0;
	m_data.mistakes = 0;
	NotifyObservers();
}

const ScoreData& ScoreModel::GetData() const
{
	return m_data;
}

ScoreData ScoreModel::GetChangedData() const
{
	return m_data;
}
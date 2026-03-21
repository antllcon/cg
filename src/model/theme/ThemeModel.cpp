#include "ThemeModel.h"

ThemeModel::ThemeModel()
{
	m_data.isDark = true;
}

void ThemeModel::Invert()
{
	m_data.isDark = !m_data.isDark;
	NotifyObservers();
}

const ThemeData& ThemeModel::GetData() const
{
	return m_data;
}

ThemeData ThemeModel::GetChangedData() const
{
	return m_data;
}
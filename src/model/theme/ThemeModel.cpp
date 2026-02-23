#include "ThemeModel.h"

namespace
{
ThemeData CreateDarkTheme()
{
	ThemeData data;
	data.windowBackground = sf::Color(30, 30, 30);
	data.surfaceBackground = sf::Color(50, 50, 50, 230);
	data.primaryText = sf::Color::White;

	return data;
}

ThemeData CreateLightTheme()
{
	ThemeData data;
	data.windowBackground = sf::Color(240, 240, 240);
	data.surfaceBackground = sf::Color(255, 255, 255, 230);
	data.primaryText = sf::Color::Black;

	return data;
}
}

ThemeModel::ThemeModel()
	: m_data(CreateDarkTheme())
	, m_isDark(true)
{
}

void ThemeModel::Toggle()
{
	m_isDark = !m_isDark;
	m_data = m_isDark ? CreateDarkTheme() : CreateLightTheme();
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
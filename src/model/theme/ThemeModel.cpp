#include "ThemeModel.h"

namespace
{
ThemeData CreateDarkTheme()
{
	ThemeData data;
	data.isDark = true;
	data.windowBackground = sf::Color(30, 30, 30, 200);
	data.surfaceBackground = sf::Color(50, 50, 50, 230);
	data.surfaceOutline = sf::Color(80, 80, 80);
	data.primaryText = sf::Color::White;

	return data;
}

ThemeData CreateLightTheme()
{
	ThemeData data;
	data.isDark = false;
	data.windowBackground = sf::Color(240, 240, 240, 200);
	data.surfaceBackground = sf::Color(255, 255, 255, 230);
	data.surfaceOutline = sf::Color(200, 200, 200);
	data.primaryText = sf::Color::Black;

	return data;
}
}

ThemeModel::ThemeModel()
	: m_data(CreateDarkTheme())
{
}

void ThemeModel::Invert()
{
	m_data = m_data.isDark ? CreateLightTheme() : CreateDarkTheme();
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
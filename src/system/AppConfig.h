#pragma once
#include "Windows.h"

#include <SFML/Graphics/Color.hpp>
#include <dwmapi.h>

namespace AppConfig
{
constexpr auto WINDOW_WIDTH = 800u;
constexpr auto WINDOW_HEIGHT = 600u;
constexpr auto FRAMERATE_LIMIT = 60u;
constexpr auto WINDOW_NAME = "Hangman";
constexpr auto BACKGROUND_COLOR = sf::Color::Black;
constexpr auto FONT_PATH = "static/font.ttf";
constexpr auto TOAST_FONT_SIZE = 12u;

constexpr auto WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
constexpr auto ICON_COLOR = sf::Color(32, 32, 32);
constexpr auto ICON_SIZE = 32u;
constexpr auto DWM_DARK_MODE_ATTRIBUTE = 20;

inline unsigned int GetMonitorRefreshRate()
{
	DEVMODE devMode;
	devMode.dmSize = sizeof(devMode);

	if (::EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode))
	{
		return static_cast<unsigned int>(devMode.dmDisplayFrequency);
	}

	return FRAMERATE_LIMIT;
}

inline void ApplyDarkTitleBar(const sf::RenderWindow& window)
{
	HWND hwnd = window.getNativeHandle();
	BOOL useDarkMode = TRUE;

	DwmSetWindowAttribute(
		hwnd,
		DWM_DARK_MODE_ATTRIBUTE,
		&useDarkMode,
		sizeof(useDarkMode));
}

inline void SetWindowIconColor(sf::RenderWindow& window, sf::Color color, unsigned int size)
{
	std::vector<uint8_t> pixels(size * size * 4);

	for (size_t i = 0; i < pixels.size(); i += 4)
	{
		pixels[i] = color.r;
		pixels[i + 1] = color.g;
		pixels[i + 2] = color.b;
		pixels[i + 3] = color.a;
	}

	window.setIcon({size, size}, pixels.data());
}
} // namespace AppConfig
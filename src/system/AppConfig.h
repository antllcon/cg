#pragma once
#include "Windows.h"

namespace AppConfig
{
constexpr auto WINDOW_WIDTH = 800u;
constexpr auto WINDOW_HEIGHT = 600u;
constexpr auto FRAMERATE_LIMIT = 60u;
constexpr auto WINDOW_NAME = "Hangman";
constexpr auto BACKGROUND_COLOR = sf::Color::Black;
constexpr auto FONT_PATH = "static/font.ttf";
constexpr auto TOAST_FONT_SIZE = 12u;

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
} // namespace AppConfig
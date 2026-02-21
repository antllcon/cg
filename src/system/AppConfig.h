#pragma once
#include "Windows.h"

namespace AppConfig
{
constexpr auto WINDOW_WIDTH = 400u;
constexpr auto WINDOW_HEIGHT = 400u;
constexpr auto FRAMERATE_LIMIT = 60u;
constexpr auto WINDOW_NAME = "Circle";
constexpr auto BACKGROUND_COLOR = sf::Color::Black;

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
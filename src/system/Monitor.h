#pragma once
#include "Windows.h"

namespace Monitor
{
constexpr auto FRAMERATE_LIMIT = 60u;

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
} // namespace Monitor
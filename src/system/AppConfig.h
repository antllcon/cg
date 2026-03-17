#pragma once
#include "src/core/types/Color.h"
#include <cstdint>
#include <Windows.h>

namespace AppConfig
{
constexpr uint32_t WINDOW_WIDTH = 400;
constexpr uint32_t WINDOW_HEIGHT = 400;
constexpr const char* WINDOW_NAME = "Circle";

constexpr uint32_t FRAMERATE_LIMIT = 144;

constexpr const char* FONT_PATH = "static/font.ttf";
constexpr uint32_t FONT_SIZE = 12;

constexpr Color ICON_COLOR_DARK{32, 32, 32, 255};
constexpr Color ICON_COLOR_LIGHT{240, 240, 240, 255};
constexpr uint32_t ICON_SIZE = 32;

namespace DarkTheme
{
constexpr Color WINDOW_BG{30, 30, 30, 200};
constexpr Color SURFACE_BG{50, 50, 50, 230};
constexpr Color SURFACE_OUTLINE{80, 80, 80, 255};
constexpr Color PRIMARY_TEXT{255, 255, 255, 255};
}

namespace LightTheme
{
constexpr Color WINDOW_BG{240, 240, 240, 200};
constexpr Color SURFACE_BG{255, 255, 255, 230};
constexpr Color SURFACE_OUTLINE{200, 200, 200, 255};
constexpr Color PRIMARY_TEXT{0, 0, 0, 255};
}
}
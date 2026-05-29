#pragma once

#include "src/utils/color/Color.h"
#include <cstdint>

namespace AppConfig
{
inline constexpr auto WINDOW_NAME = "Glow effect";
inline constexpr uint32_t WINDOW_WIDTH = 960u;
inline constexpr uint32_t WINDOW_HEIGHT = 640u;

inline constexpr auto FONT_PATH = "static/font.ttf";
inline constexpr uint32_t FONT_SIZE = 12;

inline const Color ICON_COLOR = Color::FromRGBA(240, 240, 240, 255);
inline constexpr uint8_t ICON_SIZE = 32u;

inline const Color WINDOW_BG = Color::FromRGBA(33, 33, 33);
inline const Color SURFACE_BG = Color::FromRGBA(50, 50, 50, 230);
inline const Color SURFACE_OUTLINE = Color::FromRGBA(80, 80, 80, 255);
inline const Color PRIMARY_TEXT = Color::FromRGBA(255, 255, 255, 255);

inline constexpr bool DARK_ENABLED = true;
inline constexpr bool VSYNC_ENABLED = true;
} // namespace AppConfig
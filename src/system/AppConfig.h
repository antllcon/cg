#pragma once
#include "src/core/types/color/Color.h"
#include <cstdint>

namespace AppConfig
{
inline constexpr float WINDOW_WIDTH = 800.0f;
inline constexpr float WINDOW_HEIGHT = 800.0f;
inline constexpr auto WINDOW_NAME = "Window";

inline constexpr auto FONT_PATH = "static/font.ttf";
inline constexpr uint32_t FONT_SIZE = 12;

inline constexpr auto PRESS_SOUND = "static/sound.mp3";

inline const Color ICON_COLOR_DARK = Color::FromFloat(0.125f, 0.125f, 0.125f, 1.0f);
inline const Color ICON_COLOR_LIGHT = Color::FromFloat(0.94f, 0.94f, 0.94f, 1.0f);
inline constexpr uint32_t ICON_SIZE = 32;

namespace DarkTheme
{
inline const Color WINDOW_BG = Color::FromFloat(0.53f, 0.81f, 0.92f, 1.00f);
inline const Color SURFACE_BG = Color::FromFloat(0.196f, 0.196f, 0.196f, 0.902f);
inline const Color SURFACE_OUTLINE = Color::FromFloat(0.314f, 0.314f, 0.314f, 1.0f);
inline const Color PRIMARY_TEXT = Color::FromFloat(1.0f, 1.0f, 1.0f, 1.0f);
} // namespace DarkTheme

namespace LightTheme
{
inline const Color WINDOW_BG = Color::FromFloat(1.0f, 1.0f, 1.00, 1.0f);
inline const Color SURFACE_BG = Color::FromFloat(0.94f, 0.94f, 0.94f, 0.784f);
inline const Color SURFACE_OUTLINE = Color::FromFloat(0.784f, 0.784f, 0.784f, 1.0f);
inline const Color PRIMARY_TEXT = Color::FromFloat(0.0f, 0.0f, 0.0f, 1.0f);
} // namespace LightTheme
} // namespace AppConfig
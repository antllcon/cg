#pragma once

#include "src/utils/color/Color.h"
#include <cstdint>

namespace AppConfig
{
inline constexpr auto WINDOW_NAME = "3D Maze";
inline constexpr uint32_t WINDOW_WIDTH = 960u;
inline constexpr uint32_t WINDOW_HEIGHT = 640u;

inline constexpr auto FONT_PATH = "static/font.ttf";
inline constexpr uint32_t FONT_SIZE = 12;

inline const Color ICON_COLOR = Color::FromRGBA(240, 240, 240, 255);
inline constexpr uint8_t ICON_SIZE = 32u;

inline const Color WINDOW_BG = Color::FromRGBA(8, 8, 12);
inline const Color SURFACE_BG = Color::FromRGBA(50, 50, 50, 230);
inline const Color SURFACE_OUTLINE = Color::FromRGBA(80, 80, 80, 255);
inline const Color PRIMARY_TEXT = Color::FromRGBA(255, 255, 255, 255);

inline constexpr bool DARK_ENABLED = true;
inline constexpr bool VSYNC_ENABLED = true;
} // namespace AppConfig

namespace MazeConfig
{
inline constexpr float CELL_SIZE = 2.0f;
inline constexpr float WALL_HEIGHT = 2.6f;
inline constexpr float FLOOR_THICKNESS = 0.2f;
inline constexpr float EYE_HEIGHT = 1.3f;
inline constexpr float COLLISION_RADIUS = 0.35f;

inline constexpr float MOVE_SPEED = 4.0f;
inline constexpr float MOUSE_SENSITIVITY = 0.1f;

inline const Color WALL_COLOR = Color::FromRGBA(120, 130, 150);
inline const Color FLOOR_COLOR = Color::FromRGBA(70, 60, 55);
inline const Color CEILING_COLOR = Color::FromRGBA(40, 45, 60);

inline const Color LIGHT_COLOR = Color::FromRGBA(255, 244, 214);
inline constexpr float LIGHT_AMBIENT = 0.08f;
inline constexpr float LIGHT_CONSTANT = 1.0f;
inline constexpr float LIGHT_LINEAR = 0.14f;
inline constexpr float LIGHT_QUADRATIC = 0.05f;
} // namespace MazeConfig

namespace TextureConfig
{
inline constexpr auto WALL_1 = "static/texture/wall_1.png";
inline constexpr auto WALL_2 = "static/texture/wall_2.png";
inline constexpr auto WALL_3 = "static/texture/wall_3.png";
inline constexpr auto WALL_4 = "static/texture/wall_4.png";
inline constexpr auto WALL_5 = "static/texture/wall_5.png";
inline constexpr auto WALL_6 = "static/texture/wall_6.png";
inline constexpr auto FLOOR = "static/texture/floor.png";
inline constexpr auto CEILING = "static/texture/ceiling.png";
inline constexpr auto SHADOW = "static/texture/shadow.png";
inline constexpr auto SKY = "static/texture/sky.png";
} // namespace TextureConfig

namespace RenderConfig
{
inline const Color FOG_COLOR = Color::FromRGBA(150, 160, 178);
inline constexpr float FOG_DENSITY = 0.045f;

inline constexpr float SKY_RADIUS = 50.0f;
inline constexpr int SKY_SLICES = 32;
inline constexpr int SKY_STACKS = 16;
} // namespace RenderConfig

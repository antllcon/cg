#pragma once
#include "SFML/System/Vector2.hpp"

namespace ChessGrid
{
constexpr float BOARD_OFFSET_X = 32.0f;
constexpr float BOARD_OFFSET_Y = 32.0f;
constexpr float CELL_SIZE = 42.0f;
constexpr unsigned int BOARD_SIZE = 8;

inline sf::Vector2f GridToPixels(const sf::Vector2i& gridPos)
{
	return {
		(BOARD_OFFSET_X + gridPos.x * CELL_SIZE),
		(BOARD_OFFSET_Y + gridPos.y * CELL_SIZE)};
}

inline sf::Vector2i PixelsToGrid(float x, float y)
{
	float col = (x - BOARD_OFFSET_X) / CELL_SIZE;
	float row = (y - BOARD_OFFSET_Y) / CELL_SIZE;

	return {
		static_cast<int>(std::floor(col)),
		static_cast<int>(std::floor(row))
	};
}

inline bool IsInsideBoard(const sf::Vector2i& gridPos)
{
	return gridPos.x >= 0 && gridPos.x < BOARD_SIZE &&
		   gridPos.y >= 0 && gridPos.y < BOARD_SIZE;
}
} // namespace ChessGrid
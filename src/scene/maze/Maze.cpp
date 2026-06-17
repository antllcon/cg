#include "Maze.h"
#include "MazeMap.h"
#include <cmath>
#include <stdexcept>

namespace
{
void AssertIsPositive(float value)
{
	if (value <= 0.0f)
	{
		throw std::runtime_error("Размер клетки лабиринта должен быть положительным");
	}
}

bool IsInsideBounds(int col, int row)
{
	return col >= 0 && col < MazeMap::WIDTH && row >= 0 && row < MazeMap::HEIGHT;
}

bool IsWallCell(int col, int row)
{
	if (!IsInsideBounds(col, row))
	{
		return false;
	}
	return MazeMap::CELLS[static_cast<size_t>(row)][static_cast<size_t>(col)] != 0;
}

float CellCenterOffset(int index, int count)
{
	return (static_cast<float>(index) + 0.5f - static_cast<float>(count) / 2.0f);
}

int WorldToIndex(float worldCoordinate, float cellSize, int count)
{
	return static_cast<int>(std::floor(worldCoordinate / cellSize + static_cast<float>(count) / 2.0f));
}
} // namespace

Maze::Maze(float cellSize)
	: m_cellSize(cellSize)
{
	AssertIsPositive(cellSize);
}

bool Maze::IsWall(int col, int row) const
{
	return IsWallCell(col, row);
}

bool Maze::IsBlocked(float worldX, float worldZ, float radius) const
{
	const float minX = worldX - radius;
	const float maxX = worldX + radius;
	const float minZ = worldZ - radius;
	const float maxZ = worldZ + radius;

	const int colMin = WorldToIndex(minX, m_cellSize, MazeMap::WIDTH);
	const int colMax = WorldToIndex(maxX, m_cellSize, MazeMap::WIDTH);
	const int rowMin = WorldToIndex(minZ, m_cellSize, MazeMap::HEIGHT);
	const int rowMax = WorldToIndex(maxZ, m_cellSize, MazeMap::HEIGHT);

	for (int row = rowMin; row <= rowMax; ++row)
	{
		for (int col = colMin; col <= colMax; ++col)
		{
			if (IsWallCell(col, row))
			{
				return true;
			}
		}
	}

	return false;
}

int Maze::GetColumns() const
{
	return MazeMap::WIDTH;
}

int Maze::GetRows() const
{
	return MazeMap::HEIGHT;
}

float Maze::GetCellSize() const
{
	return m_cellSize;
}

float Maze::GetWorldWidth() const
{
	return static_cast<float>(MazeMap::WIDTH) * m_cellSize;
}

float Maze::GetWorldDepth() const
{
	return static_cast<float>(MazeMap::HEIGHT) * m_cellSize;
}

Vector3 Maze::CellToWorld(int col, int row) const
{
	Vector3 result;
	result.x = CellCenterOffset(col, MazeMap::WIDTH) * m_cellSize;
	result.y = 0.0f;
	result.z = CellCenterOffset(row, MazeMap::HEIGHT) * m_cellSize;
	return result;
}

Vector3 Maze::GetSpawnPosition(float eyeHeight) const
{
	for (int row = 1; row < MazeMap::HEIGHT - 1; ++row)
	{
		for (int col = 1; col < MazeMap::WIDTH - 1; ++col)
		{
			if (!IsWallCell(col, row))
			{
				Vector3 spawn = CellToWorld(col, row);
				spawn.y = eyeHeight;
				return spawn;
			}
		}
	}

	throw std::runtime_error("В лабиринте нет свободной клетки для появления наблюдателя");
}

std::vector<Vector3> Maze::GetWallCenters() const
{
	std::vector<Vector3> centers;

	for (int row = 0; row < MazeMap::HEIGHT; ++row)
	{
		for (int col = 0; col < MazeMap::WIDTH; ++col)
		{
			if (IsWallCell(col, row))
			{
				centers.push_back(CellToWorld(col, row));
			}
		}
	}

	return centers;
}

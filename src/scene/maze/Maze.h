#pragma once

#include "src/scene/model/SceneData.h"
#include <vector>

struct WallCell
{
	Vector3 center;
	int type;
};

class Maze final
{
public:
	explicit Maze(float cellSize);
	~Maze() = default;

	bool IsWall(int col, int row) const;
	bool IsBlocked(float worldX, float worldZ, float radius) const;

	int GetColumns() const;
	int GetRows() const;
	float GetCellSize() const;
	float GetWorldWidth() const;
	float GetWorldDepth() const;

	int GetWallType(int col, int row) const;
	Vector3 CellToWorld(int col, int row) const;
	Vector3 GetSpawnPosition(float eyeHeight) const;
	std::vector<WallCell> GetWallCells() const;

private:
	float m_cellSize;
};

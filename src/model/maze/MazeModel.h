#pragma once
#include "src/core/types/math/Point.h"
#include <string>
#include <vector>

class MazeModel final
{
public:
	MazeModel();

	bool CanMoveTo(const Point3f& position) const noexcept;
	const std::vector<std::string>& GetLayout() const noexcept;
	float GetBlockSize() const noexcept;

private:
	std::vector<std::string> m_layout;
	float m_blockSize;
};
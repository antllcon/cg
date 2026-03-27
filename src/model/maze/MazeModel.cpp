#include "MazeModel.h"
#include <cmath>
#include <stdexcept>

namespace
{
void AssertIsNotEmpty(const std::vector<std::string>& layout)
{
	if (layout.empty())
	{
		throw std::runtime_error("Карта лабиринта не может быть пустой");
	}
}
}

MazeModel::MazeModel()
	: m_blockSize(2.0f)
{
	m_layout = {
		"################",
		"#.......#......#",
		"#.###.###.####.#",
		"#.#.......#....#",
		"#.#.#####.#.##.#",
		"#.#...#...#..#.#",
		"#.###.#.####.#.#",
		"#...#.#.#....#.#",
		"###.#.#.#.####.#",
		"#...#...#......#",
		"#.#####.######.#",
		"#.#...#......#.#",
		"#.#.#.######.#.#",
		"#...#......#...#",
		"##########.###.#",
		"################"
	};

	AssertIsNotEmpty(m_layout);
}

bool MazeModel::CanMoveTo(const Point3f& position) const noexcept
{
	int col = static_cast<int>(std::round(position.x / m_blockSize));
	int row = static_cast<int>(std::round(position.z / m_blockSize));

	if (row < 0 || row >= static_cast<int>(m_layout.size()))
	{
		return false;
	}

	if (col < 0 || col >= static_cast<int>(m_layout[row].size()))
	{
		return false;
	}

	return m_layout[row][col] != '#';
}

const std::vector<std::string>& MazeModel::GetLayout() const noexcept
{
	return m_layout;
}

float MazeModel::GetBlockSize() const noexcept
{
	return m_blockSize;
}
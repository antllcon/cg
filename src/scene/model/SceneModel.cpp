#include "SceneModel.h"
#include "src/system/AppConfig.h"
#include <stdexcept>

namespace
{
void AssertIsMazeValid(const Maze* maze)
{
	if (!maze)
	{
		throw std::runtime_error("В модель сцены передан невалидный лабиринт");
	}
}

SceneObject MakeWall(const Vector3& center)
{
	SceneObject wall;
	wall.shape = RenderableShape::Cube;
	wall.position = {center.x, MazeConfig::WALL_HEIGHT / 2.0f, center.z};
	wall.scale = {MazeConfig::CELL_SIZE, MazeConfig::WALL_HEIGHT, MazeConfig::CELL_SIZE};
	wall.color = MazeConfig::WALL_COLOR;
	return wall;
}

SceneObject MakeSlab(const Maze& maze, float centerY, const Color& color)
{
	SceneObject slab;
	slab.shape = RenderableShape::Cube;
	slab.position = {0.0f, centerY, 0.0f};
	slab.scale = {maze.GetWorldWidth(), MazeConfig::FLOOR_THICKNESS, maze.GetWorldDepth()};
	slab.color = color;
	return slab;
}

std::vector<SceneObject> BuildObjects(const Maze& maze)
{
	std::vector<SceneObject> objects;

	for (const auto& center : maze.GetWallCenters())
	{
		objects.push_back(MakeWall(center));
	}

	const float floorY = -MazeConfig::FLOOR_THICKNESS / 2.0f;
	const float ceilingY = MazeConfig::WALL_HEIGHT + MazeConfig::FLOOR_THICKNESS / 2.0f;
	objects.push_back(MakeSlab(maze, floorY, MazeConfig::FLOOR_COLOR));
	objects.push_back(MakeSlab(maze, ceilingY, MazeConfig::CEILING_COLOR));

	return objects;
}

Light BuildLight()
{
	Light light;
	light.position = {0.0f, MazeConfig::EYE_HEIGHT, 0.0f};
	light.color = MazeConfig::LIGHT_COLOR;
	light.ambientStrength = MazeConfig::LIGHT_AMBIENT;
	light.constant = MazeConfig::LIGHT_CONSTANT;
	light.linear = MazeConfig::LIGHT_LINEAR;
	light.quadratic = MazeConfig::LIGHT_QUADRATIC;
	return light;
}
} // namespace

SceneModel::SceneModel(std::shared_ptr<const Maze> maze)
	: m_maze(std::move(maze))
{
	AssertIsMazeValid(m_maze.get());

	m_objects = BuildObjects(*m_maze);
	m_light = BuildLight();
}

void SceneModel::Update(float /*dt*/)
{
}

std::vector<SceneObject> SceneModel::GetObjects() const
{
	return m_objects;
}

Light SceneModel::GetLight() const
{
	return m_light;
}

std::vector<SceneObject> SceneModel::GetChangedData() const
{
	return m_objects;
}

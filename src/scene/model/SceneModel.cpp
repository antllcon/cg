#include "SceneModel.h"
#include "src/system/AppConfig.h"
#include <array>
#include <stdexcept>

namespace
{
constexpr std::array<TextureKey, 6> WALL_TEXTURES = {
	TextureKey::Wall1,
	TextureKey::Wall2,
	TextureKey::Wall3,
	TextureKey::Wall4,
	TextureKey::Wall5,
	TextureKey::Wall6};

void AssertIsMazeValid(const Maze* maze)
{
	if (!maze)
	{
		throw std::runtime_error("В модель сцены передан невалидный лабиринт");
	}
}

TextureKey ResolveWallTexture(int type)
{
	const size_t index = static_cast<size_t>(type) % WALL_TEXTURES.size();
	return WALL_TEXTURES[index];
}

SceneObject MakeWall(const WallCell& cell)
{
	SceneObject wall;
	wall.shape = RenderableShape::Cube;
	wall.position = {cell.center.x, MazeConfig::WALL_HEIGHT / 2.0f, cell.center.z};
	wall.scale = {MazeConfig::CELL_SIZE, MazeConfig::WALL_HEIGHT, MazeConfig::CELL_SIZE};
	wall.texture = ResolveWallTexture(cell.type);
	wall.useShadow = true;
	return wall;
}

SceneObject MakeSlab(const Maze& maze, float centerY, TextureKey texture)
{
	SceneObject slab;
	slab.shape = RenderableShape::Cube;
	slab.position = {0.0f, centerY, 0.0f};
	slab.scale = {maze.GetWorldWidth(), MazeConfig::FLOOR_THICKNESS, maze.GetWorldDepth()};
	slab.texture = texture;
	slab.uvScaleU = maze.GetWorldWidth() / MazeConfig::CELL_SIZE;
	slab.uvScaleV = maze.GetWorldDepth() / MazeConfig::CELL_SIZE;
	return slab;
}

std::vector<SceneObject> BuildObjects(const Maze& maze)
{
	std::vector<SceneObject> objects;

	for (const auto& cell : maze.GetWallCells())
	{
		objects.push_back(MakeWall(cell));
	}

	const float floorY = -MazeConfig::FLOOR_THICKNESS / 2.0f;
	const float ceilingY = MazeConfig::WALL_HEIGHT + MazeConfig::FLOOR_THICKNESS / 2.0f;
	objects.push_back(MakeSlab(maze, floorY, TextureKey::Floor));
	objects.push_back(MakeSlab(maze, ceilingY, TextureKey::Ceiling));

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

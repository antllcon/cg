#include "SceneModel.h"
#include <cmath>
#include <stdexcept>

namespace
{
constexpr float ORBIT_RADIUS = 8.0f;
constexpr float ORBIT_SPEED = 2.0f;

constexpr Vector3 CENTER_POSITION = {0.0f, 0.0f, 0.0f};
constexpr Vector3 DEFAULT_ROTATION = {0.0f, 0.0f, 0.0f};
constexpr Vector3 DEFAULT_SCALE = {1.0f, 1.0f, 1.0f};
constexpr Vector3 TORUS_SCALE = {2.0f, 2.0f, 2.0f};
constexpr Vector3 TORUS_ROTATION = {90.0f, 0.0f, 0.0f};

constexpr Vector3 SWORD_SCALE = {0.15f, 2.5f, 0.15f};
constexpr Vector3 RED_SWORD_ROTATION = {135.0f, 45.0f, 0.0f};
constexpr Vector3 BLUE_SWORD_ROTATION = {90.0f, -45.0f, 0.0f};
constexpr Vector3 RED_SWORD_POSITION = {5.0f, 0.05f, 0.0f};
constexpr Vector3 BLUE_SWORD_POSITION = {6.0f, -0.05f, 0.0f};

const Color CYLINDER_COLOR = Color::FromFloat(1.0f, 0.3f, 0.1f);
const Color TORUS_COLOR = Color::FromFloat(0.1f, 0.1f, 0.2f);
const Color RED_SWORD_COLOR = Color::FromFloat(0.9f, 0.1f, 0.1f);
const Color BLUE_SWORD_COLOR = Color::FromFloat(0.1f, 0.3f, 1.0f);

void AssertIsFinite(float value)
{
	if (std::isnan(value) || std::isinf(value))
	{
		throw std::invalid_argument("Значение должно быть конечным числом");
	}
}

SceneObject MakeCylinder()
{
	SceneObject obj;
	obj.type = ObjectType::Cylinder;
	obj.position = CENTER_POSITION;
	obj.rotation = DEFAULT_ROTATION;
	obj.scale = DEFAULT_SCALE;
	obj.color = CYLINDER_COLOR;
	obj.isEmissive = true;
	return obj;
}

SceneObject MakeTorus()
{
	SceneObject obj;
	obj.type = ObjectType::Torus;
	obj.position = CENTER_POSITION;
	obj.rotation = TORUS_ROTATION;
	obj.scale = TORUS_SCALE;
	obj.color = TORUS_COLOR;
	obj.isEmissive = false;
	return obj;
}

SceneObject MakeRedSword()
{
	SceneObject obj;
	obj.type = ObjectType::Cylinder;
	obj.position = RED_SWORD_POSITION;
	obj.rotation = RED_SWORD_ROTATION;
	obj.scale = SWORD_SCALE;
	obj.color = RED_SWORD_COLOR;
	obj.isEmissive = true;
	return obj;
}

SceneObject MakeBlueSword()
{
	SceneObject obj;
	obj.type = ObjectType::Cylinder;
	obj.position = BLUE_SWORD_POSITION;
	obj.rotation = BLUE_SWORD_ROTATION;
	obj.scale = SWORD_SCALE;
	obj.color = BLUE_SWORD_COLOR;
	obj.isEmissive = true;
	return obj;
}

Vector3 CalculateOrbitPosition(float time)
{
	return {
		std::sin(time * ORBIT_SPEED) * ORBIT_RADIUS,
		0.0f,
		std::cos(time * ORBIT_SPEED) * ORBIT_RADIUS};
}
} // namespace

SceneModel::SceneModel()
	: m_totalTime(0.0f)
{
	m_objects.push_back(MakeCylinder());
	m_objects.push_back(MakeTorus());
	m_objects.push_back(MakeRedSword());
	m_objects.push_back(MakeBlueSword());
}

void SceneModel::Update(float dt)
{
	AssertIsFinite(dt);
	m_totalTime += dt;

	if (!m_objects.empty())
	{
		m_objects[0].position = CalculateOrbitPosition(m_totalTime);
	}
}

std::vector<SceneObject> SceneModel::GetObjects() const
{
	return m_objects;
}
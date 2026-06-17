#include "SceneModel.h"
#include <stdexcept>

namespace
{
constexpr Vector3 CENTER_POSITION = {0.0f, 0.0f, 0.0f};
constexpr Vector3 DEFAULT_ROTATION = {0.0f, 0.0f, 0.0f};
constexpr Vector3 DEFAULT_SCALE = {1.0f, 1.0f, 1.0f};

void AssertIsFinite(float value)
{
	if (std::isnan(value) || std::isinf(value))
	{
		throw std::invalid_argument("Значение должно быть конечным числом");
	}
}
} // namespace

SceneModel::SceneModel()
	: m_totalTime(0.0f)
{
	SceneObject mobius;
	mobius.shape = RenderableShape::MobiusStrip;
	mobius.position = CENTER_POSITION;
	mobius.rotation = DEFAULT_ROTATION;
	mobius.scale = DEFAULT_SCALE;
	mobius.color = Color::FromRGBA(88, 0, 255);
	m_objects.push_back(mobius);

	m_light.position = CENTER_POSITION;
	m_light.color = Color::FromRGBA(255, 255, 255);
}

void SceneModel::Update(float dt)
{
	AssertIsFinite(dt);
	m_totalTime += dt;

	if (!m_objects.empty())
	{
		NotifyObservers();
	}
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
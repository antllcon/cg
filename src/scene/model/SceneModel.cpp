#include "SceneModel.h"
#include <cmath>
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
}

void SceneModel::Update(float dt)
{
	AssertIsFinite(dt);
	m_totalTime += dt;

	if (!m_objects.empty())
	{
	}
}

std::vector<SceneObject> SceneModel::GetObjects() const
{
	return m_objects;
}
#include "Model.h"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace
{
constexpr float MAX_PITCH = 89.0f;
constexpr float MIN_PITCH = -89.0f;
constexpr float FULL_CIRCLE = 360.0f;

constexpr float ORBIT_RADIUS = 8.0f;
constexpr float ORBIT_SPEED = 2.f;

constexpr Vec3 INIT_CAMERA_POSITION = {0.0f, 0.0f, 5.0f};
constexpr float INIT_CAMERA_YAW = -90.0f;
constexpr float INIT_CAMERA_PITCH = 0.0f;
constexpr float INIT_CAMERA_FOV = 45.0f;

constexpr Vec3 CENTER_POSITION = {0.0f, 0.0f, 0.0f};
constexpr Vec3 DEFAULT_ROTATION = {0.0f, 0.0f, 0.0f};
constexpr Vec3 DEFAULT_SCALE = {1.0f, 1.0f, 1.0f};
constexpr Vec3 DARK_TORUS_SCALE = {2.0f, 2.0f, 2.0f};
constexpr Vec3 DARK_TORUS_ROTATION = {90.0f, 0.0f, 0.0f};

const Color GLOW_CYLINDER_COLOR = Color::FromFloat(1.0f, 0.3f, 0.1f);
const Color DARK_TORUS_COLOR = Color::FromFloat(0.1f, 0.1f, 0.2f);

void AssertIsFinite(float value)
{
	if (std::isnan(value) || std::isinf(value))
	{
		throw std::invalid_argument("Значение должно быть конечным числом");
	}
}
} // namespace

Model::Model()
	: m_totalTime(0.0f)
{
	InitScene();
}

void Model::MoveCamera(float dForward, float dRight, float dUp)
{
	AssertIsFinite(dForward);
	AssertIsFinite(dRight);
	AssertIsFinite(dUp);

	float yawRad = m_data.camera.yaw * (std::numbers::pi_v<float> / 180.0f);

	float forwardX = std::cos(yawRad);
	float forwardZ = std::sin(yawRad);

	float rightX = std::cos(yawRad - std::numbers::pi_v<float> / 2.0f);
	float rightZ = std::sin(yawRad - std::numbers::pi_v<float> / 2.0f);

	m_data.camera.position.x += forwardX * dForward + rightX * dRight;
	m_data.camera.position.y += dUp;
	m_data.camera.position.z += forwardZ * dForward + rightZ * dRight;

	NotifyObservers();
}

void Model::RotateCamera(float dYaw, float dPitch)
{
	AssertIsFinite(dYaw);
	AssertIsFinite(dPitch);

	// Yaw - свободное вращение на 360° с нормализацией
	m_data.camera.yaw += dYaw;
	m_data.camera.yaw = std::fmod(m_data.camera.yaw, FULL_CIRCLE);
	if (m_data.camera.yaw < 0.0f)
	{
		m_data.camera.yaw += FULL_CIRCLE;
	}

	// Pitch - ограничиваем, чтобы не переворачивать камеру (как в Minecraft)
	m_data.camera.pitch = std::clamp(m_data.camera.pitch + dPitch, MIN_PITCH, MAX_PITCH);

	NotifyObservers();
}

void Model::Update(float dt)
{
	AssertIsFinite(dt);
	m_totalTime += dt;

	if (!m_data.objects.empty())
	{
		m_data.objects[0].position.x = std::sin(m_totalTime * ORBIT_SPEED) * ORBIT_RADIUS;
		m_data.objects[0].position.z = std::cos(m_totalTime * ORBIT_SPEED) * ORBIT_RADIUS;

		NotifyObservers();
	}
}

ModelData Model::GetState() const
{
	return m_data;
}

ModelData Model::GetChangedData() const
{
	return m_data;
}

void Model::InitScene()
{
	m_data.camera.position = INIT_CAMERA_POSITION;
	m_data.camera.yaw = INIT_CAMERA_YAW;
	m_data.camera.pitch = INIT_CAMERA_PITCH;
	m_data.camera.fov = INIT_CAMERA_FOV;

	SceneObject glowCylinder;
	glowCylinder.type = ObjectType::Cylinder;
	glowCylinder.position = CENTER_POSITION;
	glowCylinder.rotation = DEFAULT_ROTATION;
	glowCylinder.scale = DEFAULT_SCALE;
	glowCylinder.color = GLOW_CYLINDER_COLOR;
	glowCylinder.isEmissive = true;

	SceneObject darkTorus;
	darkTorus.type = ObjectType::Torus;
	darkTorus.position = CENTER_POSITION;
	darkTorus.rotation = DARK_TORUS_ROTATION;
	darkTorus.scale = DARK_TORUS_SCALE;
	darkTorus.color = DARK_TORUS_COLOR;
	darkTorus.isEmissive = false;

	m_data.objects.clear();
	m_data.objects.push_back(glowCylinder);
	m_data.objects.push_back(darkTorus);
}
#include "CameraController.h"
#include "src/system/AppConfig.h"
#include "src/utils/types/event/EventHandling.h"
#include <cmath>
#include <numbers>

namespace
{
struct PlanarDelta
{
	float x;
	float z;
};

float GetMovementAxis(bool positive, bool negative)
{
	return (positive ? 1.0f : 0.0f) - (negative ? 1.0f : 0.0f);
}

PlanarDelta ComputeWorldDelta(float yaw, float forwardAxis, float rightAxis, float distance)
{
	const float yawRad = yaw * (std::numbers::pi_v<float> / 180.0f);
	const float forwardX = std::cos(yawRad);
	const float forwardZ = std::sin(yawRad);
	const float rightX = std::sin(yawRad);
	const float rightZ = -std::cos(yawRad);

	return {
		(forwardX * forwardAxis + rightX * rightAxis) * distance,
		(forwardZ * forwardAxis + rightZ * rightAxis) * distance};
}
} // namespace

CameraController::CameraController(
	std::shared_ptr<CameraModel> cameraModel,
	std::shared_ptr<const Maze> maze,
	IWindow& window)
	: m_cameraModel(std::move(cameraModel))
	, m_maze(std::move(maze))
	, m_window(window)
	, m_lastMousePos({0.0, 0.0})
	, m_firstMouseEvent(true)
{
}

void CameraController::Update(float dt)
{
	if (!m_window.IsCursorCaptured())
	{
		return;
	}

	const float forwardAxis = GetMovementAxis(m_activeKeys[KeyCode::W], m_activeKeys[KeyCode::S]);
	const float rightAxis = GetMovementAxis(m_activeKeys[KeyCode::A], m_activeKeys[KeyCode::D]);

	if (forwardAxis == 0.0f && rightAxis == 0.0f)
	{
		return;
	}

	const CameraState state = m_cameraModel->GetState();
	const float distance = MazeConfig::MOVE_SPEED * dt;
	const PlanarDelta delta = ComputeWorldDelta(state.yaw, forwardAxis, rightAxis, distance);

	float newX = state.position.x;
	float newZ = state.position.z;

	if (!m_maze->IsBlocked(state.position.x + delta.x, state.position.z, MazeConfig::COLLISION_RADIUS))
	{
		newX = state.position.x + delta.x;
	}

	if (!m_maze->IsBlocked(state.position.x, state.position.z + delta.z, MazeConfig::COLLISION_RADIUS))
	{
		newZ = state.position.z + delta.z;
	}

	if (newX != state.position.x || newZ != state.position.z)
	{
		m_cameraModel->SetPosition({newX, MazeConfig::EYE_HEIGHT, newZ});
	}
}

void CameraController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e)
				   {
					   if (e.code == KeyCode::Escape)
					   {
						   m_window.SetCursorCaptured(!m_window.IsCursorCaptured());
						   m_firstMouseEvent = true;
					   }
					   else
					   {
						   m_activeKeys[e.code] = true;
					   }
				   },
				   [this](const KeyReleasedEvent& e)
				   {
					   m_activeKeys[e.code] = false;
				   },
				   [this](const MouseMovedEvent& e)
				   {
					   if (m_firstMouseEvent)
					   {
						   m_lastMousePos = e.position;
						   m_firstMouseEvent = false;
						   return;
					   }

					   if (m_window.IsCursorCaptured())
					   {
						   const float dx = static_cast<float>(e.position.first - m_lastMousePos.first);
						   const float dy = static_cast<float>(m_lastMousePos.second - e.position.second);
						   m_cameraModel->RotateCamera(
							   dx * MazeConfig::MOUSE_SENSITIVITY,
							   dy * MazeConfig::MOUSE_SENSITIVITY);
					   }

					   m_lastMousePos = e.position;
				   },
				   [](const auto&) {}},
		event);
}

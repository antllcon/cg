#include "CameraController.h"
#include "src/utils/types/event/EventHandling.h"

namespace
{
constexpr float CAMERA_SPEED = 5.0f;
constexpr float MOUSE_SENSITIVITY = 0.1f;

float GetMovementAxis(bool positive, bool negative)
{
	return (positive ? 1.0f : 0.0f) - (negative ? 1.0f : 0.0f);
}
} // namespace

CameraController::CameraController(std::shared_ptr<CameraModel> cameraModel, IWindow& window)
	: m_cameraModel(std::move(cameraModel))
	, m_window(window)
	, m_lastMousePos({ 0.0, 0.0 })
	, m_firstMouseEvent(true)
{
}

void CameraController::Update(float dt)
{
	if (!m_window.IsCursorCaptured())
	{
		return;
	}

	float dForward = GetMovementAxis(m_activeKeys[KeyCode::W], m_activeKeys[KeyCode::S]);
	float dRight = GetMovementAxis(m_activeKeys[KeyCode::A], m_activeKeys[KeyCode::D]);
	float dUp = GetMovementAxis(m_activeKeys[KeyCode::Space], m_activeKeys[KeyCode::LShift]);

	if (dForward != 0.0f || dRight != 0.0f || dUp != 0.0f)
	{
		m_cameraModel->MoveCamera(
			dForward * CAMERA_SPEED * dt,
			dRight * CAMERA_SPEED * dt,
			dUp * CAMERA_SPEED * dt);
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
						   float dx = static_cast<float>(e.position.first - m_lastMousePos.first);
						   float dy = static_cast<float>(m_lastMousePos.second - e.position.second);
						   m_cameraModel->RotateCamera(dx * MOUSE_SENSITIVITY, dy * MOUSE_SENSITIVITY);
					   }

					   m_lastMousePos = e.position;
				   },
				   [](const auto&) {}},
		event);
}
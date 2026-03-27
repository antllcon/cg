#include "CameraController.h"
#include "src/core/interfaces/IWindow.h"
#include "src/core/types/event/EventHandling.h"
#include <algorithm>

namespace
{
constexpr float BASE_VELOCITY = 5.0f;
constexpr float MOUSE_SENSITIVITY = 0.15f;
constexpr float SPEED_MULTIPLIER_STEP = 0.2f;
constexpr float MIN_SPEED_MULTIPLIER = 0.1f;
constexpr float MAX_SPEED_MULTIPLIER = 10.0f;
} // namespace

CameraController::CameraController(std::shared_ptr<CameraModel> model, const IWindow& window)
	: m_model(std::move(model))
	, m_window(window)
{
}

void CameraController::Update(float dt)
{
	float velocity = BASE_VELOCITY * m_speedMultiplier * dt;
	float forwardAmount = 0.0f;
	float rightAmount = 0.0f;
	float upAmount = 0.0f;

	if (m_isForward) forwardAmount += velocity;
	if (m_isBackward) forwardAmount -= velocity;
	if (m_isRight) rightAmount += velocity;
	if (m_isLeft) rightAmount -= velocity;
	if (m_isUp) upAmount += velocity;
	if (m_isDown) upAmount -= velocity;

	if (forwardAmount != 0.0f || rightAmount != 0.0f || upAmount != 0.0f)
	{
		m_model->MoveLocal(forwardAmount, rightAmount, upAmount);
	}
}

void CameraController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) {
					   if (e.code == KeyCode::W) m_isForward = true;
					   if (e.code == KeyCode::S) m_isBackward = true;
					   if (e.code == KeyCode::A) m_isLeft = true;
					   if (e.code == KeyCode::D) m_isRight = true;
					   if (e.code == KeyCode::Space) m_isUp = true;
					   if (e.code == KeyCode::LShift) m_isDown = true;
				   },
				   [this](const KeyReleasedEvent& e) {
					   if (e.code == KeyCode::W) m_isForward = false;
					   if (e.code == KeyCode::S) m_isBackward = false;
					   if (e.code == KeyCode::A) m_isLeft = false;
					   if (e.code == KeyCode::D) m_isRight = false;
					   if (e.code == KeyCode::Space) m_isUp = false;
					   if (e.code == KeyCode::LShift) m_isDown = false;
				   },
				   [this](const MouseButtonPressedEvent& e) {
					   if (e.button == MouseButton::Right)
					   {
						   m_isRightMousePressed = true;
						   if (!m_window.IsCursorCaptured())
						   {
							   m_lastMousePos = Point2f(static_cast<float>(e.position.x), static_cast<float>(e.position.y));
						   }
					   }
				   },
				   [this](const MouseButtonReleasedEvent& e) {
					   if (e.button == MouseButton::Right)
					   {
						   m_isRightMousePressed = false;
					   }
				   },
				   [this](const MouseMovedEvent& e) {
					   Point2f pos(static_cast<float>(e.position.x), static_cast<float>(e.position.y));
					   bool isCaptured = m_window.IsCursorCaptured();

					   if (isCaptured && !m_wasCaptured)
					   {
						   m_lastMousePos = pos;
					   }

					   if (isCaptured || m_isRightMousePressed)
					   {
						   float xOffset = (pos.x - m_lastMousePos.x) * MOUSE_SENSITIVITY;
						   float yOffset = (m_lastMousePos.y - pos.y) * MOUSE_SENSITIVITY;
						   m_model->Rotate(xOffset, yOffset);
					   }

					   m_lastMousePos = pos;
					   m_wasCaptured = isCaptured;
				   },
				   [this](const MouseScrolledEvent& e) {
					   m_speedMultiplier += e.delta * SPEED_MULTIPLIER_STEP;
					   m_speedMultiplier = std::clamp(m_speedMultiplier, MIN_SPEED_MULTIPLIER, MAX_SPEED_MULTIPLIER);
				   },
				   [this](const WindowResizedEvent& e) {
					   if (e.height > 0)
					   {
						   m_model->SetAspectRatio(static_cast<float>(e.width) / static_cast<float>(e.height));
					   }
				   },
				   [](const auto&) {
				   }},
		event);
}
#include "CameraController.h"
#include "src/core/types/event/EventHandling.h"

namespace
{
constexpr float MOUSE_SENSITIVITY = 0.15f;
} // namespace

CameraController::CameraController(std::shared_ptr<CameraModel> model)
	: m_model(std::move(model))
	, m_isForward(false)
	, m_isBackward(false)
	, m_isLeft(false)
	, m_isRight(false)
	, m_isUp(false)
	, m_isDown(false)
	, m_isMousePressed(false)
	, m_lastMousePos(0.0f, 0.0f)
{
}

void CameraController::Update(float dt)
{
	m_fpsTimer += dt;
	m_fpsCount++;

	if (m_fpsTimer >= 1.0f)
	{
		m_model->SetFPS(m_fpsCount);
		m_fpsTimer -= 1.0f;
		m_fpsCount = 0;
	}

	float velocity = 5.0f * dt;
	float forward = 0.0f;
	float right = 0.0f;
	float up = 0.0f;

	if (m_isForward) forward += velocity;
	if (m_isBackward) forward -= velocity;
	if (m_isRight) right += velocity;
	if (m_isLeft) right -= velocity;
	if (m_isUp) up += velocity;
	if (m_isDown) up -= velocity;

	if (forward != 0.0f || right != 0.0f || up != 0.0f)
	{
		m_model->Move(forward, right, up);
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
						   m_isMousePressed = true;
						   m_lastMousePos = Point2f(static_cast<float>(e.position.x), static_cast<float>(e.position.y));
					   }
				   },
				   [this](const MouseButtonReleasedEvent& e) {
					   if (e.button == MouseButton::Right)
					   {
						   m_isMousePressed = false;
					   }
				   },
				   [this](const MouseMovedEvent& e) {
					   if (!m_isMousePressed) return;

					   Point2f currentPos(static_cast<float>(e.position.x), static_cast<float>(e.position.y));
					   float xOffset = (currentPos.x - m_lastMousePos.x) * MOUSE_SENSITIVITY;
					   float yOffset = (m_lastMousePos.y - currentPos.y) * MOUSE_SENSITIVITY;

					   m_lastMousePos = currentPos;
					   m_model->Rotate(xOffset, yOffset);
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
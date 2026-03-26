#include "CameraController.h"
#include "src/core/interfaces/IWindow.h"
#include "src/core/types/event/EventHandling.h"

CameraController::CameraController(std::shared_ptr<CameraModel> model, const IWindow& window)
	: m_model(std::move(model))
	, m_window(window)
{
}

void CameraController::Update(float dt)
{
	m_model->Update(dt);
}

void CameraController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) {
					   if (e.code == KeyCode::W) m_model->SetDirectionState(CameraDirection::Forward, true);
					   if (e.code == KeyCode::S) m_model->SetDirectionState(CameraDirection::Backward, true);
					   if (e.code == KeyCode::A) m_model->SetDirectionState(CameraDirection::Left, true);
					   if (e.code == KeyCode::D) m_model->SetDirectionState(CameraDirection::Right, true);
					   if (e.code == KeyCode::Space) m_model->SetDirectionState(CameraDirection::Up, true);
					   if (e.code == KeyCode::LShift) m_model->SetDirectionState(CameraDirection::Down, true);
				   },
				   [this](const KeyReleasedEvent& e) {
					   if (e.code == KeyCode::W) m_model->SetDirectionState(CameraDirection::Forward, false);
					   if (e.code == KeyCode::S) m_model->SetDirectionState(CameraDirection::Backward, false);
					   if (e.code == KeyCode::A) m_model->SetDirectionState(CameraDirection::Left, false);
					   if (e.code == KeyCode::D) m_model->SetDirectionState(CameraDirection::Right, false);
					   if (e.code == KeyCode::Space) m_model->SetDirectionState(CameraDirection::Up, false);
					   if (e.code == KeyCode::LShift) m_model->SetDirectionState(CameraDirection::Down, false);
				   },
				   [this](const MouseButtonPressedEvent& e) {
					   if (e.button == MouseButton::Right)
					   {
						   m_isRightMousePressed = true;
						   if (!m_window.IsCursorCaptured())
						   {
							   m_model->SetMousePressed(true, Point2f(static_cast<float>(e.position.x), static_cast<float>(e.position.y)));
						   }
					   }
				   },
				   [this](const MouseButtonReleasedEvent& e) {
					   if (e.button == MouseButton::Right)
					   {
						   m_isRightMousePressed = false;
						   if (!m_window.IsCursorCaptured())
						   {
							   m_model->SetMousePressed(false, Point2f(static_cast<float>(e.position.x), static_cast<float>(e.position.y)));
						   }
					   }
				   },
				   [this](const MouseMovedEvent& e) {
					   Point2f pos(static_cast<float>(e.position.x), static_cast<float>(e.position.y));
					   bool isCaptured = m_window.IsCursorCaptured();

					   if (isCaptured && !m_wasCaptured)
					   {
						   m_model->SetMousePressed(true, pos);
					   }
					   else if (!isCaptured && m_wasCaptured && !m_isRightMousePressed)
					   {
						   m_model->SetMousePressed(false, pos);
					   }

					   m_wasCaptured = isCaptured;
					   m_model->ProcessMouseMovement(pos);
				   },
				   [this](const MouseScrolledEvent& e) {
					   m_model->AddSpeedMultiplier(e.delta);
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
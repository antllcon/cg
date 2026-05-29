#include "Controller.h"

namespace
{
constexpr float CAMERA_SPEED = 5.0f;
}

Controller::Controller(std::shared_ptr<Model> model)
	: m_model(model)
{
}

void Controller::Update(float dt)
{
	float dForward = (m_activeKeys[KeyCode::W] ? 1.0f : 0.0f) - (m_activeKeys[KeyCode::S] ? 1.0f : 0.0f);
	float dRight = (m_activeKeys[KeyCode::A] ? 1.0f : 0.0f) - (m_activeKeys[KeyCode::D] ? 1.0f : 0.0f);
	float dUp = (m_activeKeys[KeyCode::Space] ? 1.0f : 0.0f) - (m_activeKeys[KeyCode::LShift] ? 1.0f : 0.0f);

	if (dForward != 0.0f || dRight != 0.0f || dUp != 0.0f)
	{
		m_model->MoveCamera(dForward * CAMERA_SPEED * dt, dRight * CAMERA_SPEED * dt, dUp * CAMERA_SPEED * dt);
	}

	m_model->Update(dt);
}

void Controller::HandleEvent(const Event&)
{
}

void Controller::SetKeyState(KeyCode key, bool isPressed)
{
	m_activeKeys[key] = isPressed;
}

void Controller::RotateCamera(float dx, float dy)
{
	m_model->RotateCamera(dx, dy);
}
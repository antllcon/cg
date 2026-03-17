#include "AsteroidsController.h"
#include <stdexcept>

namespace
{
void AssertIsModelValid(const std::shared_ptr<AsteroidsModel>& model)
{
	if (!model)
	{
		throw std::runtime_error("Указатель на модель не может быть пустым");
	}
}
} // namespace

AsteroidsController::AsteroidsController(std::shared_ptr<AsteroidsModel> model)
	: m_model(std::move(model))
	, m_isLeftPressed(false)
	, m_isRightPressed(false)
{
	AssertIsModelValid(m_model);
}

void AsteroidsController::Update(float dt)
{
	m_model->Update(dt);
}

void AsteroidsController::OnAcceleratePressed()
{
	m_model->SetThrust(true);
}

void AsteroidsController::OnAccelerateReleased()
{
	m_model->SetThrust(false);
}

void AsteroidsController::OnRotateLeftPressed()
{
	m_isLeftPressed = true;
	UpdateRotation();
}

void AsteroidsController::OnRotateLeftReleased()
{
	m_isLeftPressed = false;
	UpdateRotation();
}

void AsteroidsController::OnRotateRightPressed()
{
	m_isRightPressed = true;
	UpdateRotation();
}

void AsteroidsController::OnRotateRightReleased()
{
	m_isRightPressed = false;
	UpdateRotation();
}

void AsteroidsController::OnShootPressed()
{
	m_model->Shoot();
}

void AsteroidsController::UpdateRotation()
{
	float direction = 0.0f;

	if (m_isLeftPressed && !m_isRightPressed)
	{
		direction = -1.0f;
	}
	else if (m_isRightPressed && !m_isLeftPressed)
	{
		direction = 1.0f;
	}

	m_model->SetRotation(direction);
}
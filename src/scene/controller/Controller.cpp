#include "Controller.h"

Controller::Controller(std::shared_ptr<Model> model)
	: m_model(model)
{
}

void Controller::Update(float)
{
}

void Controller::HandleEvent(const Event&)
{
}

void Controller::NextFractal()
{
	m_model->NextFractalType();
}

void Controller::ResetView()
{
	m_model->ResetView();
}

void Controller::MoveCamera(float dx, float dy)
{
	m_model->MoveCamera(dx, dy);
}

void Controller::ZoomCamera(float delta)
{
	m_model->ZoomCamera(delta);
}
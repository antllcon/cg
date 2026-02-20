#include "CircleController.h"
#include "src/model/CircleModel.h"

CircleController::CircleController(std::shared_ptr<CircleModel> model)
	: m_model(std::move(model))
{
}

void CircleController::Update(float)
{
}

void CircleController::OnCanvasClicked(float x, float y)
{
	m_model->SetCenter({static_cast<int>(x), static_cast<int>(y)});
}

void CircleController::OnIncreaseRadiusClicked()
{
	m_model->IncreaseRadius(10);
}
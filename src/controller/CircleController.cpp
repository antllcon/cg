#include "CircleController.h"
#include "src/model/CircleModel.h"

#include <iostream>
#include <ostream>

namespace
{
constexpr float RADIUS_STEP = 5.0f;
}

CircleController::CircleController(std::shared_ptr<CircleModel> model)
	: m_model(std::move(model))
{
}

void CircleController::IncRadius()
{
	try
	{
		const float radius = m_model->GetRadius();
		m_model->SetRadius(radius + RADIUS_STEP);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

void CircleController::DecRadius()
{
	try
	{
		const float radius = m_model->GetRadius();
		m_model->SetRadius(radius - RADIUS_STEP);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

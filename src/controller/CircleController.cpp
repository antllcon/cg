#include "CircleController.h"

namespace
{
constexpr int SCROLL_STEP = 5;

bool IsInsideCircle(float x, float y, const CircleData& data)
{
	float dx = x - static_cast<float>(data.center.x);
	float dy = y - static_cast<float>(data.center.y);
	float radiusSq = static_cast<float>(data.radius * data.radius);

	return dx * dx + dy * dy <= radiusSq;
}
}

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

void CircleController::OnMouseScrolled(float x, float y, float delta)
{
	if (IsInsideCircle(x, y, m_model->GetData()))
	{
		int step = static_cast<int>(delta) * SCROLL_STEP;
		m_model->ChangeRadius(step);
	}
}
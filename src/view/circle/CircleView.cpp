#include "CircleView.h"
#include "src/controller/circle/CircleController.h"

CircleView::CircleView(std::shared_ptr<CircleModel> model, std::shared_ptr<CircleController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void CircleView::HandleEvent(const Event& event)
{
	m_controller->HandleEvent(event);
}

void CircleView::Update(const CircleData& data, IObservable<CircleData>*)
{
	m_data = data;
}

void CircleView::Render(IRenderer& renderer) const
{
	RenderStyle style;
	style.fillColor = m_data.fillColor;
	style.outlineColor = m_data.thicknessColor;
	style.outlineThickness = m_data.thickness;

	renderer.DrawEllipse(m_data.center, m_data.radius, style);
}
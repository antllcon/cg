#include "CircleView.h"
#include "src/controller/circle/CircleController.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/core/types/Event.h"

CircleView::CircleView(
	std::shared_ptr<CircleModel> model,
	std::shared_ptr<CircleController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void CircleView::HandleEvent(const Event& event)
{
	if (event.type == EventType::MouseButtonPressed)
	{
		bool isRightClick = event.mouseButton.button == MouseButton::Right;
		m_controller->OnCanvasClicked(
			static_cast<float>(event.mouseButton.position.x),
			static_cast<float>(event.mouseButton.position.y),
			isRightClick);
	}
	else if (event.type == EventType::MouseWheelScrolled)
	{
		m_controller->OnMouseScrolled(
			static_cast<float>(event.mouseScroll.position.x),
			static_cast<float>(event.mouseScroll.position.y),
			event.mouseScroll.delta,
			event.mouseScroll.isShiftPressed);
	}
}

void CircleView::Render(IRenderer& renderer) const
{
	renderer.DrawCircle(
		m_data.center,
		m_data.radius,
		m_data.thickness,
		m_data.fillColor,
		m_data.thicknessColor);
}

void CircleView::Update(const CircleData& data, IObservable<CircleData>*)
{
	m_data = data;
}
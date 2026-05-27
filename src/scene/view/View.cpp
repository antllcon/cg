#include "View.h"
#include "src/system/renderer/IRenderer.h"
#include "src/utils/types/event/EventHandling.h"

namespace
{
constexpr float PAN_SENSITIVITY = 0.005f;
} // namespace

View::View(std::shared_ptr<Model> model, std::shared_ptr<Controller> controller)
	: m_model(model)
	, m_controller(controller)
{
}

void View::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) {
					   if (e.code == KeyCode::Space)
					   {
						   m_controller->NextFractal();
					   }
					   else if (e.code == KeyCode::Escape || e.code == KeyCode::Delete)
					   {
						   m_controller->ResetView();
					   }
				   },
				   [this](const MouseButtonPressedEvent& e) {
					   if (e.button == MouseButton::Right)
					   {
						   m_controller->NextFractal();
					   }
					   else if (e.button == MouseButton::Left)
					   {
						   m_isDragging = true;
						   m_lastMousePos = e.position;
					   }
				   },
				   [this](const MouseButtonReleasedEvent& e) {
					   if (e.button == MouseButton::Left)
					   {
						   m_isDragging = false;
					   }
				   },
				   [this](const MouseMovedEvent& e) {
					   if (m_isDragging)
					   {
						   float dx = static_cast<float>(e.position.first - m_lastMousePos.first);
						   float dy = static_cast<float>(e.position.second - m_lastMousePos.second);

						   m_controller->MoveCamera(-dx * PAN_SENSITIVITY, dy * PAN_SENSITIVITY);
						   m_lastMousePos = e.position;
					   }
				   },
				   [this](const MouseScrolledEvent& e) {
					   m_controller->ZoomCamera(e.delta);
				   },
				   [](const auto&) {
				   }},
		event);
}

void View::Render(IRenderer& renderer) const
{
	// renderer.DrawProcedural(m_model->GetState());
}

void View::Update(const ModelData&, IObservable<ModelData>*)
{
}
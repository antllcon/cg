#include "CubeController.h"
#include "src/core/types/event/EventHandling.h"

CubeController::CubeController(std::shared_ptr<CubeModel> model)
	: m_model(std::move(model))
{
}

void CubeController::Update(float dt)
{
	m_model->Rotate(dt * 1.5f, Point3f{0.5f, 1.0f, 0.2f});
}

void CubeController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const MouseButtonPressedEvent& e) {
					   if (e.button == MouseButton::Left)
					   {
						   m_model->SetColor(Color::FromRGBA(231, 76, 60, 255));
					   }
				   },
				   [this](const MouseButtonReleasedEvent& e) {
					   if (e.button == MouseButton::Left)
					   {
						   m_model->SetColor(Color::FromRGBA(46, 204, 113, 255));
					   }
				   },
				   [](const auto&) {
				   }},
		event);
}
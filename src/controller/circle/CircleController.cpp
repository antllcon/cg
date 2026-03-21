#include "CircleController.h"
#include "src/core/types/Point.h"
#include "src/core/types/event/EventHandling.h"
#include "src/system/AppConfig.h"
#include "src/view/circle/CircleView.h"

namespace
{
constexpr float SCROLL_STEP = 5.0f;

bool IsInsideCircle(const Point2f& pos, const CircleData& data)
{
	float dx = pos.x - data.center.x;
	float dy = pos.y - data.center.y;
	float radiusSq = data.radius.x * data.radius.x;

	return dx * dx + dy * dy <= radiusSq;
}

bool IsInsideCircleThickness(const Point2f& pos, const CircleData& data)
{
	float dx = pos.x - data.center.x;
	float dy = pos.y - data.center.y;
	float totalRadius = data.radius.x + data.thickness;
	float radiusSq = totalRadius * totalRadius;

	return dx * dx + dy * dy <= radiusSq;
}
} // namespace

CircleController::CircleController(std::shared_ptr<CircleModel> model, IAudioManager* audioManager)
	: m_model(std::move(model))
	, m_audioManager(audioManager)
{
}

void CircleController::Update(float)
{
}

void CircleController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) {
					   if (e.code == KeyCode::LShift || e.code == KeyCode::RShift)
					   {
						   m_isShiftPressed = true;
					   }
				   },
				   [this](const KeyReleasedEvent& e) {
					   if (e.code == KeyCode::LShift || e.code == KeyCode::RShift)
					   {
						   m_isShiftPressed = false;
					   }
				   },
				   [this](const MouseButtonPressedEvent& e) {
					   Point2f pos{static_cast<float>(e.position.x), static_cast<float>(e.position.y)};

					   if (e.button == MouseButton::Right)
					   {
						   if (IsInsideCircle(pos, m_model->GetData()))
						   {
							   m_model->RandomFillColor();
							   throw std::runtime_error("Changed randomly color circle");
						   }

						   if (IsInsideCircleThickness(pos, m_model->GetData()))
						   {
							   m_model->RandomThicknessColor();
						   }
					   }
					   else if (e.button == MouseButton::Left)
					   {
						   m_model->SetCenter(pos);

						   if (m_audioManager)
						   {
							   m_audioManager->PlaySoundFile(AppConfig::PRESS_SOUND);
						   }
					   }
				   },
				   [this](const MouseScrolledEvent& e) {
					   Point2f pos{static_cast<float>(e.position.x), static_cast<float>(e.position.y)};

					   if (IsInsideCircleThickness(pos, m_model->GetData()))
					   {
						   float step = e.delta * SCROLL_STEP;

						   if (m_isShiftPressed)
						   {
							   m_model->ChangeThickness(step);
						   }
						   else
						   {
							   m_model->ChangeRadius(step);
						   }
					   }
				   },
				   [](const auto&) {
				   }},
		event);
}
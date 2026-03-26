#include "WindowController.h"
#include "src/core/interfaces/IWindow.h"
#include "src/core/types/event/EventHandling.h"

WindowController::WindowController(IWindow& window)
	: m_window(window)
{
}

void WindowController::Update(float)
{
}

void WindowController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) {
					   if (e.code == KeyCode::Escape)
					   {
						   m_window.SetCursorCaptured(!m_window.IsCursorCaptured());
					   }
				   },
				   [](const auto&) {
				   }},
		event);
}
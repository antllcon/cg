#include "View.h"
#include "src/system/renderer/IRenderer.h"
#include "src/utils/types/event/EventHandling.h"

namespace
{
constexpr float MOUSE_SENSITIVITY = 0.1f;
}

View::View(std::shared_ptr<Model> model, std::shared_ptr<Controller> controller)
	: m_model(model)
	, m_controller(controller)
{
}

void View::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) { HandleKeyPressed(e); },
				   [this](const KeyReleasedEvent& e) { HandleKeyReleased(e); },
				   [this](const MouseButtonPressedEvent& e) { HandleMousePressed(e); },
				   [this](const MouseButtonReleasedEvent& e) { HandleMouseReleased(e); },
				   [this](const MouseMovedEvent& e) { HandleMouseMoved(e); },
				   [](const auto&) {}},
		event);
}

void View::Render(IRenderer& renderer) const
{
	renderer.RenderFrame(m_model->GetState());
}

void View::Update(const ModelData&, IObservable<ModelData>*)
{
}

void View::HandleKeyPressed(const KeyPressedEvent& e)
{
	m_controller->SetKeyState(e.code, true);
}

void View::HandleKeyReleased(const KeyReleasedEvent& e)
{
	m_controller->SetKeyState(e.code, false);
}

void View::HandleMousePressed(const MouseButtonPressedEvent& e)
{
	if (e.button == MouseButton::Left)
	{
		m_isDragging = true;
		m_lastMousePos = e.position;
	}
}

void View::HandleMouseReleased(const MouseButtonReleasedEvent& e)
{
	if (e.button == MouseButton::Left)
	{
		m_isDragging = false;
	}
}

void View::HandleMouseMoved(const MouseMovedEvent& e)
{
	float dx = static_cast<float>(e.position.first - m_lastMousePos.first);
	float dy = static_cast<float>(m_lastMousePos.second - e.position.second);

	m_controller->RotateCamera(dx * MOUSE_SENSITIVITY, dy * MOUSE_SENSITIVITY);
	m_lastMousePos = e.position;
}
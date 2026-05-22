#include "View.h"
#include "src/system/renderer/IRenderer.h"

namespace
{
void ProcessKeyPress(const KeyPressedEvent& keyEvent, Controller& controller)
{
	switch (keyEvent.code)
	{
	case KeyCode::O:
	case KeyCode::Space:
		controller.OnOpenImageRequested();
		break;
	case KeyCode::S:
		controller.OnSaveImageRequested();
		break;
	case KeyCode::Escape:
	case KeyCode::Delete:
		controller.OnCloseImageRequested();
		break;
	case KeyCode::Up:
	case KeyCode::Left:
		controller.DecreaseFilterRadius();
		break;
	case KeyCode::Down:
	case KeyCode::Right:
		controller.IncreaseFilterRadius();
		break;
	default:
		break;
	}
}
} // namespace

View::View(std::shared_ptr<Model> model, std::shared_ptr<Controller> controller)
	: m_model(model)
	, m_controller(controller)
{
}

void View::HandleEvent(const Event& event)
{
	if (const auto* keyEvent = std::get_if<KeyPressedEvent>(&event))
	{
		ProcessKeyPress(*keyEvent, *m_controller);
	}
}

void View::Render(IRenderer& renderer) const
{
	const auto [state, image, medianRadius] = m_model->GetState();

	if (state == AppState::ImageLoaded && image)
	{
		renderer.DrawImageFiltered(image, medianRadius);
	}
	else
	{
		// TODO: отображать текст бы (открыть изображение для редактирования)
	}
}

void View::Update(const ModelData&, IObservable<ModelData>*)
{
}
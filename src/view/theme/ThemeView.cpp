#include "ThemeView.h"
#include "src/controller/theme/ThemeController.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/core/types/event/EventHandling.h"

ThemeView::ThemeView(std::shared_ptr<ThemeModel> model, std::shared_ptr<ThemeController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void ThemeView::HandleEvent(const Event& event)
{
	std::visit(Overload{
				   [this](const KeyPressedEvent& e) {
					   if (e.code == KeyCode::T)
					   {
						   m_controller->OnToggleClicked();
					   }
				   },
				   [](const auto&) {
				   }},
		event);
}

void ThemeView::Render(IRenderer&) const
{
}

void ThemeView::Update(const ThemeData&, IObservable<ThemeData>*)
{
}
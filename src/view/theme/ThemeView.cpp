#include "ThemeView.h"
#include "src/controller/theme/ThemeController.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/core/types/event/Event.h"
#include "src/core/types/event/EventHandling.h"
#include "src/system/AppConfig.h"

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

void ThemeView::Render(IRenderer& renderer) const
{
	Point2f position{0.0f, 0.0f};
	Point2f size{AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT};

	RenderStyle style;
	style.fillColor = m_backgroundColor;

	renderer.DrawRect(position, size, style);
}

void ThemeView::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	m_backgroundColor = data.windowBackground;
}
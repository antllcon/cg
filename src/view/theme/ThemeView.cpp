#include "ThemeView.h"
#include "src/controller/theme/ThemeController.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/core/types/Event.h"
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
	if (event.type == EventType::KeyPressed && event.key.code == KeyCode::T)
	{
		m_controller->OnToggleClicked();
	}
}

void ThemeView::Render(IRenderer& renderer) const
{
	renderer.DrawRectangle(
		{0, 0},
		{static_cast<int>(AppConfig::WINDOW_WIDTH), static_cast<int>(AppConfig::WINDOW_HEIGHT)},
		m_backgroundColor);
}

void ThemeView::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	m_backgroundColor = data.windowBackground;
}
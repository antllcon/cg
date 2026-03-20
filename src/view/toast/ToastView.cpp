#include "ToastView.h"
#include "src/system/AppConfig.h"

ToastView::ToastView(std::shared_ptr<ToastModel> toastModel, std::shared_ptr<ThemeModel> themeModel)
{
	if (toastModel)
	{
		Update(toastModel->GetData(), nullptr);
	}

	if (themeModel)
	{
		Update(themeModel->GetData(), nullptr);
	}
}

void ToastView::HandleEvent(const Event&)
{
}

void ToastView::Update(const ToastData& data, IObservable<ToastData>*)
{
	m_toastData = data;
}

void ToastView::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	m_themeData = data;
}

void ToastView::Render(IRenderer& renderer) const
{
	if (!m_toastData.isVisible || m_toastData.alpha <= 0.01f)
	{
		return;
	}

	Point2f size{400.0f, 50.0f};
	Point2f position{
		(AppConfig::WINDOW_WIDTH - size.x) / 2.0f,
		AppConfig::WINDOW_HEIGHT - m_toastData.offsetY};

	RenderStyle style;
	style.fillColor = Color::FromFloat(0.9f, 0.2f, 0.2f, m_toastData.alpha);

	renderer.DrawRoundedRect(position, size, 8.0f, style);

	Color textColor = Color::FromFloat(1.0f, 1.0f, 1.0f, m_toastData.alpha);

	std::string displayMessage = m_toastData.message;
	if (displayMessage.size() > 40)
	{
		displayMessage = displayMessage.substr(0, 37) + "...";
	}

	float approxTextWidth = static_cast<float>(displayMessage.size()) * (static_cast<float>(AppConfig::FONT_SIZE) * 0.55f);
	float textX = position.x + (size.x - approxTextWidth) / 2.0f;
	float textY = position.y + 30.0f;

	renderer.DrawTextData(
		Point2f{textX, textY},
		displayMessage,
		1,
		textColor);
}
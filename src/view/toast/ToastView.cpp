#include "ToastView.h"
#include "src/system/AppConfig.h"

namespace
{
constexpr float MIN_VISIBLE_ALPHA = 0.01f;
constexpr float TOAST_WIDTH = AppConfig::WINDOW_WIDTH / 2.0f;
constexpr float TOAST_HEIGHT = 30.0f;
constexpr float CORNER_RADIUS = 8.0f;
constexpr float TEXT_Y_OFFSET = 20.0f;
constexpr float FONT_WIDTH_MULTIPLIER = 0.55f;
constexpr size_t MAX_MESSAGE_LENGTH = 40;
constexpr size_t TRUNCATED_LENGTH = 37;
constexpr uint32_t DEFAULT_FONT_ID = 1;

Color GetSurfaceColor(bool isDark, float alpha)
{
	Color base = isDark ? AppConfig::DarkTheme::SURFACE_BG : AppConfig::LightTheme::SURFACE_BG;
	return Color::FromFloat(base.GetR(), base.GetG(), base.GetB(), alpha);
}

Color GetTextColor(bool isDark, float alpha)
{
	Color base = isDark ? AppConfig::DarkTheme::PRIMARY_TEXT : AppConfig::LightTheme::PRIMARY_TEXT;
	return Color::FromFloat(base.GetR(), base.GetG(), base.GetB(), alpha);
}

Point2f CalculateToastPosition(float offsetY, const Point2f& size)
{
	float x = (AppConfig::WINDOW_WIDTH - size.x) / 2.0f;
	float y = AppConfig::WINDOW_HEIGHT - offsetY;
	return Point2f{x, y};
}

std::string TruncateMessage(const std::string& message)
{
	if (message.size() > MAX_MESSAGE_LENGTH)
	{
		return message.substr(0, TRUNCATED_LENGTH) + "...";
	}
	return message;
}

float CalculateTextXOffset(const std::string& text, float toastWidth)
{
	float approxTextWidth = static_cast<float>(text.size()) * (static_cast<float>(AppConfig::FONT_SIZE) * FONT_WIDTH_MULTIPLIER);
	return (toastWidth - approxTextWidth) / 2.0f;
}

void RenderSurface(IRenderer& renderer, const Point2f& position, bool isDark, float alpha)
{
	Point2f size{TOAST_WIDTH, TOAST_HEIGHT};
	RenderStyle style;
	style.fillColor = GetSurfaceColor(isDark, alpha);

	renderer.DrawRoundedRect(position, size, CORNER_RADIUS, style);
}

void RenderText(IRenderer& renderer, const Point2f& position, const std::string& message, bool isDark, float alpha)
{
	std::string displayMessage = TruncateMessage(message);
	float textX = position.x + CalculateTextXOffset(displayMessage, TOAST_WIDTH);
	float textY = position.y + TEXT_Y_OFFSET;
	Color textColor = GetTextColor(isDark, alpha);

	renderer.DrawTextData(Point2f{textX, textY}, displayMessage, DEFAULT_FONT_ID, textColor);
}
} // namespace

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

void ToastView::Render(IRenderer& renderer) const
{
	if (!m_toastData.isVisible || m_toastData.alpha <= MIN_VISIBLE_ALPHA)
	{
		return;
	}

	Point2f size{TOAST_WIDTH, TOAST_HEIGHT};
	Point2f position = CalculateToastPosition(m_toastData.offsetY, size);

	RenderSurface(renderer, position, m_themeData.isDark, m_toastData.alpha);
	RenderText(renderer, position, m_toastData.message, m_themeData.isDark, m_toastData.alpha);
}

void ToastView::Update(const ToastData& data, IObservable<ToastData>*)
{
	m_toastData = data;
}

void ToastView::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	m_themeData = data;
}
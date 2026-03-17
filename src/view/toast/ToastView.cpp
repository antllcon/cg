#include "ToastView.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/system/AppConfig.h"

namespace
{
constexpr float PADDING_X = 20.0f;
constexpr float PADDING_Y = 10.0f;
constexpr float OFFSET_BOTTOM = 30.0f;
constexpr float CORNER_RADIUS = 12.0f;

float EstimateTextWidth(const std::string& text, uint32_t fontSize)
{
	return static_cast<float>(text.length()) * (static_cast<float>(fontSize) * 0.6f);
}
} // namespace

ToastView::ToastView(std::shared_ptr<ToastModel> toastModel, std::shared_ptr<ThemeModel> themeModel)
	: m_isVisible(false)
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
	if (m_isVisible)
	{
		renderer.DrawRoundedRectangle(
			{static_cast<int>(m_position.x), static_cast<int>(m_position.y)},
			{static_cast<int>(m_size.x), static_cast<int>(m_size.y)},
			CORNER_RADIUS,
			m_backgroundColor,
			m_outlineColor);

		renderer.DrawTextData(
			{static_cast<int>(m_position.x + PADDING_X), static_cast<int>(m_position.y + PADDING_Y)},
			m_message,
			static_cast<float>(AppConfig::FONT_SIZE),
			m_textColor);
	}
}

void ToastView::Update(const ToastData& data, IObservable<ToastData>*)
{
	m_isVisible = data.isVisible;

	if (m_isVisible)
	{
		m_message = data.message;
		CalculateBounds();
	}
}

void ToastView::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	m_textColor = data.primaryText;
	m_backgroundColor = data.surfaceBackground;
	m_outlineColor = data.surfaceOutline;
}

void ToastView::CalculateBounds()
{
	float textWidth = EstimateTextWidth(m_message, AppConfig::FONT_SIZE);
	float textHeight = static_cast<float>(AppConfig::FONT_SIZE);

	m_size.x = textWidth + PADDING_X * 2.0f;
	m_size.y = textHeight + PADDING_Y * 2.0f;

	m_position.x = (static_cast<float>(AppConfig::WINDOW_WIDTH) - m_size.x) / 2.0f;
	m_position.y = static_cast<float>(AppConfig::WINDOW_HEIGHT) - m_size.y - OFFSET_BOTTOM;
}
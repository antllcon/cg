#include "FpsView.h"
#include "src/core/types/color/Color.h"
#include <string>

namespace
{
constexpr float FPS_PANEL_X = 10.0f;
constexpr float FPS_PANEL_Y = 10.0f;
constexpr float FPS_PANEL_WIDTH = 70.0f;
constexpr float FPS_PANEL_HEIGHT = 24.0f;
constexpr float CORNER_RADIUS = 6.0f;

constexpr float TEXT_OFFSET_X = 8.0f;
constexpr float TEXT_OFFSET_Y = 16.0f;

constexpr auto FPS_TEXT = "FPS: ";

const Color PANEL_BG_COLOR = Color::FromRGBA(30, 30, 30, 200);
const Color PANEL_OUTLINE_COLOR = Color::FromRGBA(100, 100, 100, 255);
const Color TEXT_COLOR = Color::FromRGBA(255, 215, 0, 255);
}

FpsView::FpsView(std::shared_ptr<FpsModel> model)
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void FpsView::HandleEvent(const Event&)
{
}

void FpsView::Render(IRenderer& renderer) const
{
	RenderStyle style;
	style.fillColor = PANEL_BG_COLOR;
	style.outlineColor = PANEL_OUTLINE_COLOR;

	Point2f position{FPS_PANEL_X, FPS_PANEL_Y};
	Point2f size{FPS_PANEL_WIDTH, FPS_PANEL_HEIGHT};

	renderer.DrawRoundedRect(position, size, CORNER_RADIUS, style);

	std::string text = FPS_TEXT + std::to_string(m_data.fps);
	Point2f textPos{FPS_PANEL_X + TEXT_OFFSET_X, FPS_PANEL_Y + TEXT_OFFSET_Y};

	renderer.DrawTextData(textPos, text, 1, TEXT_COLOR);
}

void FpsView::Update(const FpsData& data, IObservable<FpsData>*)
{
	m_data = data;
}
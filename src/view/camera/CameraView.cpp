#include "CameraView.h"
#include "src/controller/camera/CameraController.h"
#include "src/core/types/color/Color.h"
#include <string>

namespace
{
constexpr float FPS_PANEL_X = 10.0f;
constexpr float FPS_PANEL_Y = 10.0f;
constexpr float FPS_PANEL_WIDTH = 80.0f;
constexpr float FPS_PANEL_HEIGHT = 24.0f;
constexpr float CORNER_RADIUS = 6.0f;

constexpr float TEXT_OFFSET_X = 8.0f;
constexpr float TEXT_OFFSET_Y = 16.0f;

constexpr auto FPS_TEXT = "FPS: ";

const Color PANEL_BG_COLOR = Color::FromRGBA(30, 30, 30, 200);
const Color PANEL_OUTLINE_COLOR = Color::FromRGBA(100, 100, 100, 255);
const Color TEXT_COLOR = Color::FromRGBA(255, 215, 0, 255); // Золотой
} // namespace

CameraView::CameraView(std::shared_ptr<CameraModel> model, std::shared_ptr<CameraController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void CameraView::HandleEvent(const Event& event)
{
	m_controller->HandleEvent(event);
}

void CameraView::Render(IRenderer& renderer) const
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

void CameraView::Update(const CameraData& data, IObservable<CameraData>*)
{
	m_data = data;
}
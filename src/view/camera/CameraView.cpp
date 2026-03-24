#include "CameraView.h"
#include "src/controller/camera/CameraController.h"
#include "src/core/types/color/Color.h"
#include <string>

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
	std::string text = "FPS: " + std::to_string(m_data.fps);
	renderer.DrawTextData(Point2f{10.0f, 20.0f}, text, 1, Color::FromFloat(1.0f, 1.0f, 0.0f, 1.0f));
}

void CameraView::Update(const CameraData& data, IObservable<CameraData>*)
{
	m_data = data;
}
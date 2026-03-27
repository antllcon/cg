#include "CameraView.h"
#include "src/controller/camera/CameraController.h"

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

void CameraView::Render(IRenderer&) const
{
}

void CameraView::Update(const CameraData& data, IObservable<CameraData>*)
{
	m_data = data;
}
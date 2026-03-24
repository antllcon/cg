#pragma once
#include "src/core/interfaces/IRenderer.h"
#include "src/model/camera/CameraModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>

class CameraController;

class CameraView final
	: public IView
	, public IObserver<CameraData>
{
public:
	CameraView(std::shared_ptr<CameraModel> model, std::shared_ptr<CameraController> controller);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const CameraData& data, IObservable<CameraData>* subject) override;

private:
	std::shared_ptr<CameraController> m_controller;
	CameraData m_data;
};
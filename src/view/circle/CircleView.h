#pragma once
#include "../../model/circle/CircleModel.h"
#include "../IView.h"
#include "src/system/Observer.h"
#include <memory>

class CircleController;

class CircleView final
	: public IView
	, public IObserver<CircleData>
{
public:
	CircleView(std::shared_ptr<CircleModel> model, std::shared_ptr<CircleController> controller);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const CircleData& data, IObservable<CircleData>* subject) override;

private:
	std::shared_ptr<CircleController> m_controller;
	CircleData m_data;
};
#pragma once
#include "src/model/asteroids/AsteroidsModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>

class AsteroidsController;

class AsteroidsView final
	: public IView
	, public IObserver<AsteroidsData>
{
public:
	AsteroidsView(std::shared_ptr<AsteroidsModel> model, std::shared_ptr<AsteroidsController> controller);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const AsteroidsData& data, IObservable<AsteroidsData>* subject) override;

private:
	std::shared_ptr<AsteroidsController> m_controller;
	AsteroidsData m_data;
};
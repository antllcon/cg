#pragma once
#include "src/core/interfaces/IRenderer.h"
#include "src/model/fps/FpsModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>

class FpsView final
	: public IView
	, public IObserver<FpsData>
{
public:
	explicit FpsView(std::shared_ptr<FpsModel> model);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const FpsData& data, IObservable<FpsData>* subject) override;

private:
	FpsData m_data;
};
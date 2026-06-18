#pragma once

#include "src/scene/model/Model.h"
#include "src/scene/view/IView.h"
#include "src/utils/Observer.h"
#include <memory>

class View final
	: public IView
	, public IObserver<ModelData>
{
public:
	explicit View(std::shared_ptr<Model> model);
	~View() override = default;

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const ModelData& data, IObservable<ModelData>* subject) override;

private:
	std::shared_ptr<Model> m_model;
};
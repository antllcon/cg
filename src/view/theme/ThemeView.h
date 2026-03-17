#pragma once
#include "src/model/theme/ThemeModel.h"
#include "src/view/IView.h"
#include "src/system/Observer.h"
#include <memory>

class ThemeController;

class ThemeView final
	: public IView
	, public IObserver<ThemeData>
{
public:
	ThemeView(std::shared_ptr<ThemeModel> model, std::shared_ptr<ThemeController> controller);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const ThemeData& data, IObservable<ThemeData>* subject) override;

private:
	std::shared_ptr<ThemeController> m_controller;
	Color m_backgroundColor;
};
#pragma once
#include "src/core/types/Point.h"
#include "src/model/theme/ThemeModel.h"
#include "src/model/toast/ToastModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>
#include <string>

class ToastView final
	: public IView
	, public IObserver<ToastData>
	, public IObserver<ThemeData>
{
public:
	ToastView(std::shared_ptr<ToastModel> toastModel, std::shared_ptr<ThemeModel> themeModel);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const ToastData& data, IObservable<ToastData>* subject) override;
	void Update(const ThemeData& data, IObservable<ThemeData>* subject) override;

private:
	void CalculateBounds();

	std::string m_message;
	bool m_isVisible;

	Color m_textColor;
	Color m_backgroundColor;
	Color m_outlineColor;

	Point2f m_position;
	Point2f m_size;
};
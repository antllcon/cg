#pragma once
#include "src/core/interfaces/IRenderer.h"
#include "src/model/toast/ToastModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>

class ToastView final
	: public IView
	, public IObserver<ToastData>
{
public:
	explicit ToastView(std::shared_ptr<ToastModel> toastModel);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const ToastData& data, IObservable<ToastData>* subject) override;

private:
	ToastData m_toastData;
};

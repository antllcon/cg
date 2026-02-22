#include "MainScene.h"
#include "src/controller/CircleController.h"
#include "src/model/CircleModel.h"
#include "src/view/CircleView.h"
#include "src/view/ToastView.h"

void MainScene::Init()
{
	auto circleModel = std::make_shared<CircleModel>();
	AddModel(circleModel);

	auto circleController = std::make_shared<CircleController>(circleModel);
	AddController(circleController);

	auto circleView = std::make_shared<CircleView>(circleModel, circleController);
	circleModel->RegisterObserver(circleView);
	AddView(circleView);

	auto toastModel = std::make_shared<ToastModel>();
	AddModel(toastModel);

	m_toastController = std::make_shared<ToastController>(toastModel);
	AddController(m_toastController);

	auto toastView = std::make_shared<ToastView>(toastModel);
	toastModel->RegisterObserver(toastView);
	AddView(toastView);
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}
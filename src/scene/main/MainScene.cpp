#include "MainScene.h"
#include "../../controller/circle/CircleController.h"
#include "../../controller/theme/ThemeController.h"
#include "../../model/circle/CircleModel.h"
#include "../../view/circle/CircleView.h"
#include "../../view/theme/ThemeView.h"
#include "../../view/toast/ToastView.h"

void MainScene::Init()
{
	auto themeModel = std::make_shared<ThemeModel>();
	AddModel(themeModel);

	auto themeController = std::make_shared<ThemeController>(themeModel);
	AddController(themeController);

	auto themeView = std::make_shared<ThemeView>(themeModel, themeController);
	themeModel->RegisterObserver(themeView);
	AddView(themeView);

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

	auto toastView = std::make_shared<ToastView>(toastModel, themeModel);
	toastModel->RegisterObserver(toastView);
	themeModel->RegisterObserver(toastView);
	AddView(toastView);
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}
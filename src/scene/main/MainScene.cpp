#include "MainScene.h"
#include "src/controller/asteroids/AsteroidsController.h"
#include "src/controller/theme/ThemeController.h"
#include "src/model/asteroids/AsteroidsModel.h"
#include "src/view/asteroids/AsteroidsView.h"
#include "src/view/theme/ThemeView.h"
#include "src/view/toast/ToastView.h"

void MainScene::Init(std::shared_ptr<ThemeModel> themeModel)
{
	auto themeController = std::make_shared<ThemeController>(themeModel);
	AddController(themeController);

	auto themeView = std::make_shared<ThemeView>(themeModel, themeController);
	themeModel->RegisterObserver(themeView);
	AddView(themeView);

	auto asteroidsModel = std::make_shared<AsteroidsModel>();
	AddModel(asteroidsModel);

	auto asteroidsController = std::make_shared<AsteroidsController>(asteroidsModel);
	AddController(asteroidsController);

	auto asteroidsView = std::make_shared<AsteroidsView>(asteroidsModel, asteroidsController);
	asteroidsModel->RegisterObserver(asteroidsView);
	AddView(asteroidsView);

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
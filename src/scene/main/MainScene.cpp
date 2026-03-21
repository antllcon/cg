#include "MainScene.h"
#include "src/controller/circle/CircleController.h"
#include "src/controller/game/AsteroidsController.h"
#include "src/controller/theme/ThemeController.h"
#include "src/model/game/AsteroidsModel.h"
#include "src/view/circle/CircleView.h"
#include "src/view/game/AsteroidsView.h"
#include "src/view/theme/ThemeView.h"
#include "src/view/toast/ToastView.h"

void MainScene::Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager& audioManager)
{
	auto themeController = std::make_shared<ThemeController>(themeModel);
	AddController(themeController);

	auto themeView = std::make_shared<ThemeView>(themeModel, themeController);
	themeModel->RegisterObserver(themeView);
	AddView(themeView);

	auto toastModel = std::make_shared<ToastModel>();
	AddModel(toastModel);

	m_toastController = std::make_shared<ToastController>(toastModel);
	AddController(m_toastController);

	auto toastView = std::make_shared<ToastView>(toastModel, themeModel);
	toastModel->RegisterObserver(toastView);
	themeModel->RegisterObserver(toastView);
	AddView(toastView);

	auto asteroidsModel = std::make_shared<AsteroidsModel>();
	AddModel(asteroidsModel);

	auto asteroidsController = std::make_shared<AsteroidsController>(asteroidsModel, audioManager);
	AddController(asteroidsController);

	auto asteroidsView = std::make_shared<AsteroidsView>(asteroidsModel, asteroidsController, themeModel);
	asteroidsModel->RegisterObserver(asteroidsView);
	themeModel->RegisterObserver(asteroidsView);
	AddView(asteroidsView);
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}
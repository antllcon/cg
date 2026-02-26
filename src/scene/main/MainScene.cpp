#include "MainScene.h"
#include "../../controller/game/KeyboardController.h"
#include "../../controller/theme/ThemeController.h"
#include "../../model/game/KeyboardModel.h"
#include "../../model/game/WordModel.h"
#include "../../view/game/KeyboardView.h"
#include "../../view/theme/ThemeView.h"
#include "../../view/toast/ToastView.h"

void MainScene::Init(std::shared_ptr<ThemeModel> themeModel)
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

    auto wordModel = std::make_shared<WordModel>();
    AddModel(wordModel);
    wordModel->SetNewRiddle({"MVC", "Паттерн проектирования", "Model-View-Controller"});

    auto keyboardModel = std::make_shared<KeyboardModel>();
    AddModel(keyboardModel);

    auto keyboardController = std::make_shared<KeyboardController>(keyboardModel, wordModel);
    AddController(keyboardController);

	auto keyboardView = std::make_shared<KeyboardView>(keyboardController, keyboardModel->GetData());
    keyboardModel->RegisterObserver(keyboardView);
    AddView(keyboardView);
}

void MainScene::OnException(const std::exception& e)
{
    if (m_toastController)
    {
        m_toastController->ShowError(e.what());
    }
}
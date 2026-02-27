#include "MainScene.h"
#include "../../controller/game/GameController.h"
#include "../../controller/game/KeyboardController.h"
#include "../../controller/game/ScoreController.h"
#include "../../controller/game/WordController.h"
#include "../../controller/theme/ThemeController.h"
#include "../../model/game/KeyboardModel.h"
#include "../../model/game/ScoreModel.h"
#include "../../model/game/WordModel.h"
#include "../../view/game/KeyboardView.h"
#include "../../view/game/ScoreView.h"
#include "../../view/game/WordView.h"
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

	auto keyboardModel = std::make_shared<KeyboardModel>();
	AddModel(keyboardModel);

	auto scoreModel = std::make_shared<ScoreModel>();
	AddModel(scoreModel);

	std::vector<Riddle> riddles = {
		{"MVC", "Паттерн проектирования", "Model-View-Controller"},
		{"CPP", "Язык программирования", "С плюсами"},
		{"SFML", "Мультимедийная библиотека", "Simple and Fast"},
		{"GAMEDEV", "Разработка игр", "То, чем мы сейчас занимаемся"},
		{"TEMPLATE", "Средство обобщенного программирования", "Мощный инструмент C++"},
		{"LAMBDA", "Анонимная функция в коде", "Захватывает контекст"},
		{"POINTER", "Переменная, хранящая адрес памяти", "Бывает сырым и умным"},
		{"RECURSION", "Вызов функции из самой себя", "Нужно условие выхода"}};

	auto wordController = std::make_shared<WordController>(wordModel);
	AddController(wordController);

	auto scoreController = std::make_shared<ScoreController>(scoreModel);
	AddController(scoreController);

	auto keyboardController = std::make_shared<KeyboardController>(keyboardModel);
	AddController(keyboardController);

	auto gameController = std::make_shared<GameController>(keyboardController, wordController, scoreController, riddles);
	AddController(gameController);

	keyboardController->SetLetterCallback([weakGameCtrl = std::weak_ptr<GameController>(gameController)](char letter) {
		if (auto ctrl = weakGameCtrl.lock())
		{
			ctrl->ProcessLetter(letter);
		}
	});

	auto wordView = std::make_shared<WordView>(wordModel->GetData(), themeModel->GetData());
	wordModel->RegisterObserver(wordView);
	themeModel->RegisterObserver(wordView);
	AddView(wordView);

	auto keyboardView = std::make_shared<KeyboardView>(keyboardController, keyboardModel->GetData());
	keyboardModel->RegisterObserver(keyboardView);
	AddView(keyboardView);

	auto scoreView = std::make_shared<ScoreView>(scoreModel->GetData(), themeModel->GetData());
	scoreModel->RegisterObserver(scoreView);
	themeModel->RegisterObserver(scoreView);
	AddView(scoreView);

	gameController->StartGame();
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}
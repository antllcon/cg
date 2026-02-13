#include "MainScene.h"
#include "src/controller/LettersController.h"
#include "src/model/LettersModel.h"
#include "src/view/LettersView.h"

void MainScene::Init()
{
	auto model = std::make_shared<LettersModel>();
	AddModel(model);

	auto controller = std::make_shared<LettersController>(model);
	AddController(controller);

	auto view = std::make_shared<LettersView>(model);
	AddView(view);
	model->RegisterObserver(view);
}
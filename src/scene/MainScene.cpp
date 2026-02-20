#include "MainScene.h"
#include "src/controller/CircleController.h"
#include "src/model/CircleModel.h"
#include "src/view/CircleView.h"

void MainScene::Init()
{
	auto circleModel = std::make_shared<CircleModel>();
	AddModel(circleModel);

	auto circleController = std::make_shared<CircleController>(circleModel);
	AddController(circleController);

	auto circleView = std::make_shared<CircleView>(circleModel, circleController);
	circleModel->RegisterObserver(circleView);
	AddView(circleView);
}
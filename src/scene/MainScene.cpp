#include "MainScene.h"
#include "src/controller/PiecesController.h"
#include "src/model/PiecesModel.h"
#include "src/view/BoardView.h"
#include "src/view/PiecesView.h"

void MainScene::Init()
{
	auto chessModel = std::make_shared<PiecesModel>();
	AddModel(chessModel);

	auto piecesController = std::make_shared<PiecesController>(chessModel);
	AddController(piecesController);

	auto boardView = std::make_shared<BoardView>();
	AddView(boardView);

	auto piecesView = std::make_shared<PiecesView>(chessModel, piecesController);
	chessModel->RegisterObserver(piecesView);
	AddView(piecesView);
}
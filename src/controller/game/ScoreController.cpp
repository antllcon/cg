#include "ScoreController.h"
#include "src/model/game/ScoreModel.h"
#include <stdexcept>

namespace
{
void AssertIsModelValid(const void* model)
{
	if (model == nullptr)
	{
		throw std::runtime_error("Указатель на модель не может быть пустым");
	}
}
}

ScoreController::ScoreController(std::shared_ptr<ScoreModel> model)
	: m_model(model)
{
	AssertIsModelValid(m_model.get());
}

void ScoreController::Update(float)
{
}

void ScoreController::AddWin()
{
	m_model->AddWin();
}

bool ScoreController::AddMistakeAndCheckLoss()
{
	return m_model->AddMistakeAndCheckLoss();
}

void ScoreController::Reset()
{
	m_model->Reset();
}
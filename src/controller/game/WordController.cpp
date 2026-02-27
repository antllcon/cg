#include "WordController.h"
#include "src/model/game/WordModel.h"
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

WordController::WordController(std::shared_ptr<WordModel> model)
	: m_model(model)
{
	AssertIsModelValid(m_model.get());
}

void WordController::Update(float)
{
}

void WordController::SetNewRiddle(const Riddle& riddle)
{
	m_model->SetNewRiddle(riddle);
}

bool WordController::OpenLetter(char letter)
{
	return m_model->OpenLetter(letter);
}

bool WordController::IsFullyGuessed() const
{
	return m_model->IsFullyGuessed();
}
#pragma once
#include "../IController.h"
#include <memory>

class ScoreModel;

class ScoreController final : public IController
{
public:
	explicit ScoreController(std::shared_ptr<ScoreModel> model);

	void Update(float dt) override;
	void AddWin();
	bool AddMistakeAndCheckLoss();
	void Reset();

private:
	std::shared_ptr<ScoreModel> m_model;
};
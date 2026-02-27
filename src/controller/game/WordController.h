#pragma once
#include "../IController.h"
#include "src/model/game/GameTypes.h"
#include <memory>

class WordModel;

class WordController final : public IController
{
public:
	explicit WordController(std::shared_ptr<WordModel> model);

	void Update(float dt) override;
	void SetNewRiddle(const Riddle& riddle);
	bool OpenLetter(char letter);
	bool IsFullyGuessed() const;

private:
	std::shared_ptr<WordModel> m_model;
};
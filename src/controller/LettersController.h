#pragma once
#include "IController.h"
#include <memory>

class LettersModel;

class LettersController final : public IController
{
public:
	explicit LettersController(std::shared_ptr<LettersModel> model);

	void Update(float dt) override;

private:
	std::shared_ptr<LettersModel> m_model;
};
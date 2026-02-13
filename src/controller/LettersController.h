#pragma once
#include "IController.h"
#include <memory>
#include <vector>

class LettersModel;

class LettersController final : public IController
{
public:
	explicit LettersController(std::shared_ptr<LettersModel> model);

	void Update(float dt) override;

private:
	void InitTimers();
	bool UpdateLetterState(size_t index, float dt);

	std::shared_ptr<LettersModel> m_model;
	std::vector<float> m_letterTimers;
};
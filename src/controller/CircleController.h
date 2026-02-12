#pragma once
#include "IController.h"
#include <memory>

class CircleModel;

class CircleController final : public IController
{
public:
	explicit CircleController(std::shared_ptr<CircleModel> model);

	void Update(float dt) override;
	void IncRadius();
	void DecRadius();

private:
	std::shared_ptr<CircleModel> m_model;
};

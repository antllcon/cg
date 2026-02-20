#pragma once
#include "IController.h"
#include "src/model/CircleModel.h"
#include <memory>

class CircleController final : public IController
{
public:
	explicit CircleController(std::shared_ptr<CircleModel> model);

	void Update(float) override;

	void OnCanvasClicked(float x, float y);
	void OnMouseScrolled(float x, float y, float delta);

private:
	std::shared_ptr<CircleModel> m_model;
};
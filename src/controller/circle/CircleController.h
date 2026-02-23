#pragma once
#include "../../model/circle/CircleModel.h"
#include "../IController.h"
#include <memory>

class CircleController final : public IController
{
public:
	explicit CircleController(std::shared_ptr<CircleModel> model);

	void Update(float) override;

	void OnCanvasClicked(float x, float y, bool isRightClick);
	void OnMouseScrolled(float x, float y, float delta, bool isShiftPressed);

private:
	std::shared_ptr<CircleModel> m_model;
};
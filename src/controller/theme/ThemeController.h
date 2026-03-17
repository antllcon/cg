#pragma once
#include "src/controller/IController.h"
#include "src/model/theme/ThemeModel.h"
#include <memory>

class ThemeController final : public IController
{
public:
	explicit ThemeController(std::shared_ptr<ThemeModel> model);

	void Update(float dt) override;
	void OnToggleClicked();

private:
	std::shared_ptr<ThemeModel> m_model;
};
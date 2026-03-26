#pragma once
#include "src/controller/IController.h"
#include "src/model/sun/SunModel.h"

#include <memory>

class SunController final : public IController
{
public:
	explicit SunController(std::shared_ptr<SunModel> model);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<SunModel> m_model;
};
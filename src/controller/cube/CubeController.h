#pragma once
#include "src/controller/IController.h"
#include "src/model/cube/CubeModel.h"
#include <memory>

class CubeController final : public IController
{
public:
	explicit CubeController(std::shared_ptr<CubeModel> model);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<CubeModel> m_model;
};
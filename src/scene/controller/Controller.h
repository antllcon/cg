#pragma once

#include "src/scene/controller/IController.h"

class Controller final : public IController
{
public:
	Controller() = default;
	~Controller() override = default;

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;
};
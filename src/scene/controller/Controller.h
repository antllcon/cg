#pragma once

#include "src/scene/controller/IController.h"
#include "src/scene/model/Model.h"
#include <map>
#include <memory>

class Controller final : public IController
{
public:
	explicit Controller(std::shared_ptr<Model> model);
	~Controller() override = default;

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

	void SetKeyState(KeyCode key, bool isPressed);
	void RotateCamera(float dx, float dy);

private:
	std::shared_ptr<Model> m_model;
	std::map<KeyCode, bool> m_activeKeys;
};
#pragma once
#include "src/controller/IController.h"
#include "src/model/asteroids/AsteroidsModel.h"
#include <memory>

class AsteroidsController final : public IController
{
public:
	explicit AsteroidsController(std::shared_ptr<AsteroidsModel> model);

	void Update(float dt) override;

	void OnAcceleratePressed();
	void OnAccelerateReleased();
	void OnRotateLeftPressed();
	void OnRotateLeftReleased();
	void OnRotateRightPressed();
	void OnRotateRightReleased();
	void OnShootPressed();

private:
	void UpdateRotation();

	std::shared_ptr<AsteroidsModel> m_model;
	bool m_isLeftPressed;
	bool m_isRightPressed;
};
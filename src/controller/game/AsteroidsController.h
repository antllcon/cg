#pragma once
#include "src/controller/IController.h"
#include "src/core/interfaces/IAudioManager.h"
#include "src/model/game/AsteroidsModel.h"
#include <memory>

class AsteroidsController final : public IController
{
public:
	AsteroidsController(std::shared_ptr<AsteroidsModel> model, IAudioManager& audioManager);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<AsteroidsModel> m_model;
	IAudioManager& m_audioManager;
};
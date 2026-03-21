#pragma once
#include "src/controller/IController.h"
#include "src/core/interfaces/IAudioManager.h"
#include "src/core/types/event/Event.h"
#include "src/model/circle/CircleModel.h"
#include <memory>

class CircleController final : public IController
{
public:
	explicit CircleController(std::shared_ptr<CircleModel> model, IAudioManager& audioManager);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<CircleModel> m_model;
	IAudioManager& m_audioManager;
	bool m_isShiftPressed = false;
};
#pragma once

#include "IController.h"
#include "src/scene/model/SceneModel.h"
#include <memory>

class SceneController final : public IController
{
public:
	explicit SceneController(std::shared_ptr<SceneModel> sceneModel);
	~SceneController() override = default;

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<SceneModel> m_sceneModel;
};
#pragma once
#include "src/controller/IController.h"
#include "src/model/camera/CameraModel.h"
#include <memory>

class CameraController final : public IController
{
public:
	explicit CameraController(std::shared_ptr<CameraModel> model);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<CameraModel> m_model;
};
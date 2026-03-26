#pragma once
#include "src/controller/IController.h"
#include "src/model/camera/CameraModel.h"
#include <memory>

class IWindow;

class CameraController final : public IController
{
public:
	CameraController(std::shared_ptr<CameraModel> model, const IWindow& window);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<CameraModel> m_model;
	const IWindow& m_window;
	bool m_isRightMousePressed{false};
	bool m_wasCaptured{false};
};
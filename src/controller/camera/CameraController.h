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

	bool m_isForward{false};
	bool m_isBackward{false};
	bool m_isLeft{false};
	bool m_isRight{false};
	bool m_isUp{false};
	bool m_isDown{false};

	float m_speedMultiplier{1.2f};
	bool m_isRightMousePressed{false};
	bool m_wasCaptured{false};
	Point2f m_lastMousePos{0.0f, 0.0f};
};
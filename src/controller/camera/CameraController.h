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

	bool m_isForward;
	bool m_isBackward;
	bool m_isLeft;
	bool m_isRight;
	bool m_isUp;
	bool m_isDown;

	bool m_isMousePressed;
	Point2f m_lastMousePos;

	float m_fpsTimer{0.0f};
	uint32_t m_fpsCount{0};
};
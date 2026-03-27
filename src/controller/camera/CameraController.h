#pragma once
#include "src/controller/IController.h"
#include "src/core/interfaces/IAudioManager.h"
#include "src/model/camera/CameraModel.h"
#include "src/model/maze/MazeModel.h"
#include <memory>

class IWindow;

class CameraController final : public IController
{
public:
	CameraController(
		std::shared_ptr<CameraModel> model,
		std::shared_ptr<MazeModel> mazeModel,
		const IWindow& window,
		IAudioManager& audioManager);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<CameraModel> m_model;
	std::shared_ptr<MazeModel> m_mazeModel;
	const IWindow& m_window;
	IAudioManager& m_audioManager;

	bool m_isForward{false};
	bool m_isBackward{false};
	bool m_isLeft{false};
	bool m_isRight{false};
	bool m_isUp{false};
	bool m_isDown{false};

	float m_speedMultiplier{1.0f};
	bool m_isRightMousePressed{false};
	bool m_wasCaptured{false};
	Point2f m_lastMousePos{0.0f, 0.0f};

	bool m_isSpectator{false};
};
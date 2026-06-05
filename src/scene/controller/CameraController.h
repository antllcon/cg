#pragma once

#include "IController.h"
#include "src/scene/model/CameraModel.h"
#include "src/system/window/IWindow.h"
#include <map>
#include <memory>

class CameraController final : public IController
{
public:
	CameraController(std::shared_ptr<CameraModel> cameraModel, IWindow& window);
	~CameraController() override = default;

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	std::shared_ptr<CameraModel> m_cameraModel;
	IWindow& m_window;
	std::map<KeyCode, bool> m_activeKeys;
	std::pair<double, double> m_lastMousePos;
	bool m_firstMouseEvent;
};
#pragma once
#include "src/controller/toast/ToastController.h"
#include "src/model/camera/CameraModel.h"
#include "src/model/fps/FpsModel.h"
#include "src/model/maze/MazeModel.h"
#include "src/scene/Scene.h"

class MainScene final : public Scene
{
public:
	void Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager& audioManager, IWindow& window) override;
	void Update(float dt) override;
	void OnException(const std::exception& e) override;

private:
	void GenerateLevel();

	std::shared_ptr<ToastController> m_toastController;
	std::shared_ptr<CameraModel> m_cameraModel;
	std::shared_ptr<FpsModel> m_fpsModel;
	std::shared_ptr<LightModel> m_torchModel;
	std::shared_ptr<MazeModel> m_mazeModel;
};
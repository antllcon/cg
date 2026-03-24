#pragma once
#include "src/controller/toast/ToastController.h"
#include "src/core/types/scene/CameraData.h"
#include "src/core/types/scene/LightData.h"
#include "src/scene/Scene.h"
#include "src/view/cube/CubeView.h"
#include "src/view/theme/ThemeView.h"
#include "src/view/toast/ToastView.h"

class MainScene final : public Scene
{
public:
	void Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager& audioManager) override;
	void Render(IRenderer& renderer) const override;
	void OnException(const std::exception& e) override;

private:
	std::shared_ptr<ToastController> m_toastController;

	std::shared_ptr<ThemeView> m_themeView;
	std::shared_ptr<ToastView> m_toastView;
	std::shared_ptr<CubeView> m_cubeView;

	CameraData m_camera;
	LightData m_light;
};
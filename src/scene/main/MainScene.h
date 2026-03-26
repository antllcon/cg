#pragma once
#include "src/controller/toast/ToastController.h"
#include "src/model/camera/CameraModel.h"
#include "src/scene/Scene.h"
#include "src/view/IView.h"
#include <vector>

class MainScene final : public Scene
{
public:
	void Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager& audioManager, IWindow& window) override;
	void Render(IRenderer& renderer) const override;
	void OnException(const std::exception& e) override;

private:
	std::shared_ptr<ToastController> m_toastController;
	std::shared_ptr<CameraModel> m_cameraModel;

	std::vector<std::shared_ptr<IView>> m_uiViews;
	std::vector<std::shared_ptr<IView>> m_entityViews;
};
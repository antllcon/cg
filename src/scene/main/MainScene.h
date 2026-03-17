#pragma once
#include "src/controller/toast/ToastController.h"
#include "src/scene/Scene.h"

class MainScene final : public Scene
{
public:
	void Init(std::shared_ptr<ThemeModel> themeModel) override;
	void OnException(const std::exception& e) override;

private:
	std::shared_ptr<ToastController> m_toastController;
};
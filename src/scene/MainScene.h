#pragma once
#include "Scene.h"
#include "src/controller/ToastController.h"

class MainScene final : public Scene
{
public:
	void Init() override;
	void OnException(const std::exception& e) override;

private:
	std::shared_ptr<ToastController> m_toastController;
};
#pragma once
#include "src/controller/IController.h"
#include "src/model/toast/ToastModel.h"
#include <memory>
#include <string>

class ToastController final : public IController
{
public:
	explicit ToastController(std::shared_ptr<ToastModel> model);

	void Update(float dt) override;
	void ShowError(const std::string& errorMessage);

private:
	std::shared_ptr<ToastModel> m_model;
	float m_timer;
};
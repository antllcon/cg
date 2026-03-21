#pragma once
#include "src/controller/IController.h"
#include "src/core/interfaces/INotificationService.h"
#include "src/model/toast/ToastModel.h"
#include <memory>
#include <string>

class ToastController final
	: public IController
	, public INotificationService
{
public:
	explicit ToastController(std::shared_ptr<ToastModel> model);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;
	void ShowMessage(const std::string& message) override;
	void ShowError(const std::string& errorMessage) override;

private:
	std::shared_ptr<ToastModel> m_model;
	float m_timer;
};
#pragma once

#include "src/scene/controller/IController.h"
#include "src/scene/model/Model.h"
#include <memory>

class Controller final : public IController
{
public:
	explicit Controller(std::shared_ptr<Model> model);
	~Controller() override = default;

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

	void OnOpenImageRequested();
	void OnCloseImageRequested();
	void OnSaveImageRequested();

	void IncreaseFilterRadius();
	void DecreaseFilterRadius();

private:
	std::shared_ptr<Model> m_model;
};
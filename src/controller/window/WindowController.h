#pragma once
#include "src/controller/IController.h"

class IWindow;

class WindowController final : public IController
{
public:
	explicit WindowController(IWindow& window);

	void Update(float dt) override;
	void HandleEvent(const Event& event) override;

private:
	IWindow& m_window;
};
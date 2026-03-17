#pragma once
#include "src/core/interfaces/IWindow.h"
#include <queue>

struct GLFWWindow;

class GlfwWindow final : public IWindow
{
public:
	GlfwWindow();
	~GlfwWindow() override;

	bool IsOpen() const override;
	void Close() override;
	bool PollEvent(Event& event) override;
	Point2i GetSize() const override;
	void SetTitleBarTheme(bool isDark) override;
	void SetIconColor(const Color& color) override;

	void PushEvent(const Event& event);

private:
	GLFWWindow* m_window;
	std::queue<Event> m_events;
};
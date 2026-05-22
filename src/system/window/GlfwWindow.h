#pragma once

#include "IWindow.h"
#include <queue>

struct GLFWwindow;

class GlfwWindow : public IWindow
{
public:
	GlfwWindow();
	~GlfwWindow() override;

	bool IsOpen() const override;
	void Close() override;

	std::optional<Event> PollEvent() override;
	void PushEvent(Event event);

	std::pair<uint16_t, uint16_t> GetSize() const override;

	void SetTitleBarColor(bool isDark) override;
	void SetIcon(uint8_t size, const Color& color) override;

	void SetVSync(bool enabled) override;
	void SetFullscreen(bool isFullscreen) override;

	void SetCursorCaptured(bool captured) override;
	bool IsCursorCaptured() const override;

private:
	GLFWwindow* m_window;
	std::queue<Event> m_events;
	bool m_isCursorCaptured{false};
};
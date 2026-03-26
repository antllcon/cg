#pragma once
#include "src/core/interfaces/IWindow.h"
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
	void PushEvent(const Event& event);

	Point2i GetSize() const override;
	void SetTitleBarTheme(bool isDark) override;

	void SetIconColor(const Color& color) override;
	void SetIconFromFile(const std::filesystem::path& path) override;

	void SetVSync(bool enabled) override;
	void SetFullscreen(bool isFullscreen) override;

	void SetCursorCaptured(bool captured) override;
	bool IsCursorCaptured() const override;

private:
	GLFWwindow* m_window;
	std::queue<Event> m_events;
	bool m_isCursorCaptured{false};
};
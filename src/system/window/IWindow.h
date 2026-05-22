#pragma once

#include "src/utils/color/Color.h"
#include "src/utils/types/event/Event.h"
#include <filesystem>
#include <optional>

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual bool IsOpen() const = 0;
	virtual void Close() = 0;

	virtual std::optional<Event> PollEvent() = 0;
	virtual void WaitEvents() = 0;

	virtual void SwapBuffers() = 0;

	virtual std::pair<uint16_t, uint16_t> GetSize() const = 0;
	virtual void SetTitleBarColor(bool isDark) = 0;
	virtual void SetIcon(uint8_t size, const Color& color) = 0;

	virtual void SetVSync(bool enabled) = 0;
	virtual void SetFullscreen(bool isFullscreen) = 0;

	virtual void SetCursorCaptured(bool captured) = 0;
	virtual bool IsCursorCaptured() const = 0;
};
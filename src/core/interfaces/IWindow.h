#pragma once
#include "src/core/types/color/Color.h"
#include "src/core/types/event/Event.h"
#include <filesystem>
#include <optional>

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual bool IsOpen() const = 0;
	virtual void Close() = 0;

	virtual std::optional<Event> PollEvent() = 0;

	virtual Point2i GetSize() const = 0;
	virtual void SetTitleBarTheme(bool isDark) = 0;

	virtual void SetIconColor(const Color& color) = 0;
	virtual void SetIconFromFile(const std::filesystem::path& path) = 0;

	virtual void SetVSync(bool enabled) = 0;
	virtual void SetFullscreen(bool isFullscreen) = 0;

	virtual void SetCursorCaptured(bool captured) = 0;
	virtual bool IsCursorCaptured() const = 0;
};
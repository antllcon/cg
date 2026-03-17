#pragma once
#include "../types/Event.h"
#include "../types/Point.h"
#include "src/core/types/Color.h"

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual bool IsOpen() const = 0;
	virtual void Close() = 0;
	virtual bool PollEvent(Event& event) = 0;

	virtual Point2i GetSize() const = 0;
	virtual void SetTitleBarTheme(bool isDark) = 0;
	virtual void SetIconColor(const Color& color) = 0;
};
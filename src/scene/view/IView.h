#pragma once

#include "src/utils/types/event/Event.h"

class IRenderer;

class IView
{
public:
	virtual ~IView() = default;

	virtual void HandleEvent(const Event& event) = 0;
	virtual void Render(IRenderer& renderer) const = 0;
};
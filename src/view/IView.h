#pragma once
#include "src/core/interfaces/IRenderer.h"
#include "src/core/types/event/Event.h"

class IView
{
public:
	virtual ~IView() = default;

	virtual void HandleEvent(const Event& event) = 0;
	virtual void Render(IRenderer& renderer) const = 0;
};
#pragma once

#include "src/utils/types/event/Event.h"

class IController
{
public:
	virtual ~IController() = default;

	virtual void Update(float dt) = 0;
	virtual void HandleEvent(const Event& event) = 0;
};
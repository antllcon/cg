#pragma once

class IModel
{
public:
	virtual ~IModel() = default;

	virtual void UpdateState(float dt) = 0;
	virtual float GetState() const = 0;
};
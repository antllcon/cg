#pragma once
#include "../system/Observer.h"

struct CircleData
{
	float radius;
};

class CircleModel final : public CObservable<CircleData>
{
public:
	CircleModel();

	void SetRadius(const float& radius);
	float GetRadius() const;

protected:
	CircleData GetChangedData() const override;

private:
	CircleData m_data;
};
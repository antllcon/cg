#pragma once
#include "src/system/Observer.h"
#include <cstdint>

struct FpsData
{
	uint32_t fps{0};
};

class FpsModel final : public CObservable<FpsData>
{
public:
	void Update(float dt);
	const FpsData& GetData() const;

protected:
	FpsData GetChangedData() const override;

private:
	FpsData m_data;
	float m_timer{0.0f};
	uint32_t m_frameCount{0};
};
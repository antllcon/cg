#pragma once
#include "src/core/types/Point.h"
#include "src/core/types/color/Color.h"
#include "src/core/types/scene/LightData.h"
#include "src/system/Observer.h"

class LightModel : public CObservable<LightData>
{
public:
	LightModel();

	void SetType(LightType type);
	void SetColor(const Color& color);
	void SetPosition(const Point3f& position);
	void SetDirection(const Point3f& direction);
	void SetIntensity(float intensity);
	void SetRange(float range);

	const LightData& GetData() const;

protected:
	LightData GetChangedData() const override;

	LightData m_data;
};
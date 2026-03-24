#pragma once
#include "src/core/types/color/Color.h"
#include "src/core/types/math/Transform.h"
#include "src/system/Observer.h"

struct CubeData
{
	Transform transform;
	Color color;
};

class CubeModel final : public CObservable<CubeData>
{
public:
	CubeModel();

	void Rotate(float angle, const Point3f& axis);
	void SetColor(const Color& color);
	const CubeData& GetData() const;

protected:
	CubeData GetChangedData() const override;

private:
	CubeData m_data;
};
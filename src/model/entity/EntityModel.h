#pragma once
#include "src/core/types/color/Color.h"
#include "src/core/types/math/Transform.h"
#include "src/system/Observer.h"

struct EntityData
{
	Transform transform;
	Color color;
};

class EntityModel final : public CObservable<EntityData>
{
public:
	EntityModel();

	void SetPosition(const Point3f& position);
	void SetScale(const Point3f& scale);
	void Rotate(float angle, const Point3f& axis);
	void SetColor(const Color& color);

	const EntityData& GetData() const;

protected:
	EntityData GetChangedData() const override;

private:
	EntityData m_data;
};
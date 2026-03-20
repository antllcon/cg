#pragma once
#include "src/core/types/Point.h"
#include "src/core/types/color/Color.h"
#include "src/system/Observer.h"

struct CircleData
{
	Point2f center;
	Point2f radius;
	float thickness;
	Color thicknessColor; // вот это уровень представления VIEW модели должно быть пофиг на цвет, ей важны физические данные!!!
	Color fillColor;
};

class CircleModel final : public CObservable<CircleData>
{
public:
	CircleModel();

	void SetCenter(const Point2f& center);
	void ChangeRadius(float delta);
	void ChangeThickness(float delta);
	void RandomFillColor();
	void RandomThicknessColor();

	const CircleData& GetData() const;

protected:
	CircleData GetChangedData() const override;

private:
	CircleData m_data;
};
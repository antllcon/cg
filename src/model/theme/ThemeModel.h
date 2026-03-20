#pragma once
#include "src/core/types/color/Color.h"
#include "src/system/Observer.h"

struct ThemeData
{
	bool isDark;
	Color windowBackground;
	Color surfaceBackground;
	Color surfaceOutline;
	Color primaryText;
};

class ThemeModel final : public CObservable<ThemeData>
{
public:
	ThemeModel();

	void Invert();
	const ThemeData& GetData() const;

protected:
	ThemeData GetChangedData() const override;

private:
	ThemeData m_data;
};
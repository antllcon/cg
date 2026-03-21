#pragma once
#include "src/system/Observer.h"

struct ThemeData
{
	bool isDark;
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
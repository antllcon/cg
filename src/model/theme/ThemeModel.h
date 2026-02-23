#pragma once
#include "SFML/Graphics/Color.hpp"
#include "src/system/Observer.h"

struct ThemeData
{
	sf::Color windowBackground;
	sf::Color surfaceBackground;
	sf::Color primaryText;
};

class ThemeModel final : public CObservable<ThemeData>
{
public:
	ThemeModel();

	void Toggle();
	const ThemeData& GetData() const;

protected:
	ThemeData GetChangedData() const override;

private:
	ThemeData m_data;
	bool m_isDark;
};
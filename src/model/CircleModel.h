#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "src/system/Observer.h"

struct CircleData
{
	sf::Vector2i center;
	int radius;
	sf::Color color;
};

class CircleModel final : public CObservable<CircleData>
{
public:
	CircleModel();

	void SetCenter(const sf::Vector2i& center);
	void ChangeRadius(int delta);
	const CircleData& GetData() const;

protected:
	CircleData GetChangedData() const override;

private:
	CircleData m_data;
};
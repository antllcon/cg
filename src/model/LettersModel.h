#pragma once
#include "../system/Observer.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"

enum class LetterType
{
	G,
	S,
	V,
	O
};

struct LetterData
{
	LetterType type;
	sf::Vector2f position;
	sf::Color color;
};

using LettersData = std::vector<LetterData>;

class LettersModel final : public CObservable<LettersData>
{
public:
	LettersModel();

	LettersData& GetData();
	const LettersData& GetData() const;

protected:
	LettersData GetChangedData() const override;

private:
	LettersData m_data;
};
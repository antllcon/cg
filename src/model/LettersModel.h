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
	sf::Vector2f basePosition;
	sf::Color color;
};

using LettersData = std::vector<LetterData>;

class LettersModel final : public CObservable<LettersData>
{
public:
	LettersModel();

	const LettersData& GetData() const;
	void SetLetterPosition(size_t index, const sf::Vector2f& position);

	void BeginUpdate();
	void EndUpdate();

protected:
	LettersData GetChangedData() const override;

private:
	LettersData m_data;
	bool m_isUpdating;
};
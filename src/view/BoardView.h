#pragma once
#include "IView.h"

class BoardView final : public IView
{
public:
	void HandleEvent(const sf::Event&, const sf::RenderWindow&) override;
	void Render(sf::RenderWindow& window) const override;
};
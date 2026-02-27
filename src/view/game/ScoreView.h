#pragma once
#include "src/model/game/ScoreModel.h"
#include "src/model/theme/ThemeModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <SFML/Graphics.hpp>

class ScoreView final
	: public IView
	, public IObserver<ScoreData>
	, public IObserver<ThemeData>
{
public:
	ScoreView(const ScoreData& initialData, const ThemeData& initialTheme);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(const ScoreData& data, IObservable<ScoreData>* subject) override;
	void Update(const ThemeData& data, IObservable<ThemeData>* subject) override;

private:
	sf::Font m_font;
	sf::Text m_scoreText;

	void UpdateLayout(unsigned int windowWidth, unsigned int windowHeight);
};
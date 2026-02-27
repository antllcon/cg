#pragma once
#include "src/model/game/WordModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <SFML/Graphics.hpp>

class WordView final
	: public IView
	, public IObserver<WordData>
{
public:
	explicit WordView(const WordData& initialData);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(const WordData& data, IObservable<WordData>* subject) override;

private:
	sf::Font m_font;
	sf::Text m_descriptionText;
	sf::Text m_hintText;
	sf::Text m_wordText;

	void UpdateLayout(unsigned int windowWidth, unsigned int windowHeight);
	static std::string AddSpaces(const std::string& word);
};
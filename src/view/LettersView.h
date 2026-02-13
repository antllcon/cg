#pragma once
#include "IView.h"
#include "src/model/LettersModel.h"
#include "src/system/Observer.h"

class LettersView final
	: public IView
	, public IObserver<LettersData>
{
public:
	explicit LettersView(std::shared_ptr<LettersModel> model);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(std::vector<LetterData> const& data, IObservable<std::vector<LetterData>>*) override;

private:
	void AddRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
	void AddCircle(const sf::Vector2f& pos, float radius, const sf::Color& color);

	void DrawG(const LetterData& data);
	void DrawS(const LetterData& data);
	void DrawV(const LetterData& data);
	void DrawO(const LetterData& data);

	std::vector<std::shared_ptr<sf::Drawable>> m_shapes;
};
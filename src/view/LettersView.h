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

	void HandleEvent(const sf::Event&, const sf::RenderWindow&) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(std::vector<LetterData> const& data, IObservable<std::vector<LetterData>>*) override;

private:
	std::vector<std::unique_ptr<sf::Drawable>> m_shapes;
};
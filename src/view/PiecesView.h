#pragma once
#include "IView.h"
#include "src/model/PiecesModel.h"
#include "src/system/Observer.h"
#include "SFML/Graphics/Drawable.hpp"

class PiecesController;

class PiecesView final
	: public IView
	, public IObserver<ChessData>
{
public:
	PiecesView(std::shared_ptr<PiecesModel> model, std::shared_ptr<PiecesController> controller);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(const ChessData& data, IObservable<ChessData>*) override;

private:
	std::shared_ptr<PiecesController> m_controller;
	std::vector<std::unique_ptr<sf::Drawable>> m_shapes;
};
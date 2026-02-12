#pragma once
#include "../system/Observer.h"
#include "IView.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "src/controller/CircleController.h"
#include "src/model/CircleModel.h"

class CircleView final :public IView, public IObserver<CircleData>
{
public:
	CircleView(std::shared_ptr<CircleController> controller, std::shared_ptr<CircleModel> model);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(CircleData const& data, IObservable<CircleData>*) override;

private:
	std::shared_ptr<CircleController> m_controller;
	sf::CircleShape m_circle;
};
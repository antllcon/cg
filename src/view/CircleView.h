#pragma once
#include "IView.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "src/model/CircleModel.h"
#include "src/system/Observer.h"
#include <memory>
#include <vector>

class CircleController;

class CircleView final
	: public IView
	, public IObserver<CircleData>
{
public:
	CircleView(std::shared_ptr<CircleModel> model, std::shared_ptr<CircleController> controller);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(const CircleData& data, IObservable<CircleData>* subject) override;

private:
	void SetupButton();
	bool IsButtonClicked(float x, float y) const;
	void UpdateVertices(const std::vector<sf::Vector2i>& points, const sf::Color& color);

	std::shared_ptr<CircleController> m_controller;
	sf::VertexArray m_vertices;
	sf::RectangleShape m_button;
};
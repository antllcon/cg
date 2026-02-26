#pragma once
#include "src/controller/game/KeyboardController.h"
#include "src/model/game/KeyboardModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>

struct KeyVisuals
{
	sf::RectangleShape shape;
	sf::Text text;
	bool isEnabled;
};

class KeyboardView final
	: public IView
	, public IObserver<KeyboardData>
{
public:
	KeyboardView(std::shared_ptr<KeyboardController> controller, const KeyboardData& initialData);
	~KeyboardView() override;

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(const KeyboardData& data, IObservable<KeyboardData>* subject) override;

private:
	std::shared_ptr<KeyboardController> m_controller;
	std::unordered_map<char, KeyVisuals> m_keys;
	sf::Font m_font;

	void InitKeys();
	void HandleMouseClick(const sf::RenderWindow& window);
	void ProcessKeyClick(char letter);
	void ApplyLetterState(char letter, LetterStatus status);
};
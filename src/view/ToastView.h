#pragma once
#include "IView.h"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "src/model/ToastModel.h"
#include "src/system/Observer.h"
#include <memory>

class ToastView final
	: public IView
	, public IObserver<ToastData>
{
public:
	explicit ToastView(std::shared_ptr<ToastModel> model);

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) const override;
	void Update(const ToastData& data, IObservable<ToastData>* subject) override;

private:
	void SetupVisuals(const std::string& message);

	sf::Font m_font;
	sf::Text m_text;
	sf::ConvexShape m_background;
	bool m_isVisible;
};
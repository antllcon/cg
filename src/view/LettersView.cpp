#include "LettersView.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "cmake-build-debug-vs/_deps/sfml-src/include/SFML/Graphics/CircleShape.hpp"

namespace
{
constexpr float LETTER_HEIGHT = 100.0f;
constexpr float LETTER_WIDTH = 80.0f;
constexpr float THICKNESS = 20.0f;

constexpr sf::Color BG_COLOR = sf::Color::Black;
} // namespace
LettersView::LettersView(std::shared_ptr<LettersModel> model)
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void LettersView::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	(void)event;
	(void)window;
}

void LettersView::Render(sf::RenderWindow& window) const
{
	for (const auto& shape : m_shapes)
	{
		window.draw(*shape);
	}
}

void LettersView::Update(LettersData const& data, IObservable<std::vector<LetterData>>*)
{
	m_shapes.clear();
	m_shapes.reserve(data.size() * 4);

	for (const auto& letter : data)
	{
		switch (letter.type)
		{
		case LetterType::G:
			DrawG(letter);
			break;
		case LetterType::S:
			DrawS(letter);
			break;
		case LetterType::V:
			DrawV(letter);
			break;
		case LetterType::O:
			DrawO(letter);
			break;
		}
	}
}

void LettersView::AddRect(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
	auto rect = std::make_shared<sf::RectangleShape>(size);
	rect->setPosition(pos);
	rect->setFillColor(color);
	m_shapes.push_back(rect);
}

void LettersView::AddCircle(const sf::Vector2f& pos, float radius, const sf::Color& color)
{
	auto circle = std::make_shared<sf::CircleShape>(radius);
	circle->setPosition(pos);
	circle->setFillColor(color);
	m_shapes.push_back(circle);
}

void LettersView::DrawG(const LetterData& data)
{
	AddRect(data.position, {THICKNESS, LETTER_HEIGHT}, data.color);
	AddRect(data.position, {LETTER_WIDTH, THICKNESS}, data.color);
}

void LettersView::DrawS(const LetterData& data)
{
	constexpr float outerRadius = LETTER_HEIGHT / 2.0f;
	constexpr float innerRadius = outerRadius - THICKNESS;

	AddCircle(data.position, outerRadius, data.color);

	sf::Vector2f innerPos = {
		data.position.x + THICKNESS,
		data.position.y + THICKNESS};
	AddCircle(innerPos, innerRadius, BG_COLOR);

	sf::Vector2f maskPos = {
		data.position.x + outerRadius,
		data.position.y + THICKNESS};
	sf::Vector2f maskSize = {
		outerRadius + 10.0f,
		LETTER_HEIGHT - (2 * THICKNESS)};

	AddRect(maskPos, maskSize, BG_COLOR);
}

void LettersView::DrawV(const LetterData& data)
{

	float bottomHeight = LETTER_HEIGHT * 0.6f;
	float bottomWidth = LETTER_WIDTH;
	sf::Vector2f bottomPos = {data.position.x, data.position.y + (LETTER_HEIGHT - bottomHeight)};

	AddRect(bottomPos, {bottomWidth, bottomHeight}, data.color);

	AddRect(
		{bottomPos.x + THICKNESS, bottomPos.y + THICKNESS},
		{bottomWidth - 2 * THICKNESS, bottomHeight - 2 * THICKNESS},
		BG_COLOR);

	float topHeight = LETTER_HEIGHT * 0.4f;
	float topWidth = LETTER_WIDTH * 0.8f;

	sf::Vector2f topPos = data.position;

	AddRect(topPos, {topWidth, topHeight}, data.color);

	AddRect(
		{topPos.x + THICKNESS, topPos.y + THICKNESS},
		{topWidth - 2 * THICKNESS, (topHeight - 2 * THICKNESS) * 2},
		BG_COLOR);
}

void LettersView::DrawO(const LetterData& data)
{
	constexpr float outerRadius = LETTER_HEIGHT / 2.0f;
	constexpr float innerRadius = outerRadius - THICKNESS;

	AddCircle(data.position, outerRadius, data.color);

	sf::Vector2f innerPos = {
		data.position.x + THICKNESS,
		data.position.y + THICKNESS};
	AddCircle(innerPos, innerRadius, BG_COLOR);
}
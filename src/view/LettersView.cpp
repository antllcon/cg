#include "LettersView.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "ShapeFactory.h"

namespace
{
constexpr auto LETTER_HEIGHT = 100.0f;
constexpr auto LETTER_WIDTH = 80.0f;
constexpr auto THICKNESS = 20.0f;
constexpr auto BG_COLOR = sf::Color::Black;

void DrawG(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const LetterData& data)
    {
        shapes.push_back(ShapeFactory::CreateRect(data.position, {THICKNESS, LETTER_HEIGHT}, data.color));
        shapes.push_back(ShapeFactory::CreateRect(data.position, {LETTER_WIDTH, THICKNESS}, data.color));
    }

    void DrawS(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const LetterData& data)
    {
        constexpr float outerRadius = LETTER_HEIGHT / 2.0f;
        constexpr float innerRadius = outerRadius - THICKNESS;

        shapes.push_back(ShapeFactory::CreateCircle(data.position, outerRadius, data.color));

        sf::Vector2f innerPos = {data.position.x + THICKNESS, data.position.y + THICKNESS};
        shapes.push_back(ShapeFactory::CreateCircle(innerPos, innerRadius, BG_COLOR));

        sf::Vector2f maskPos = {data.position.x + outerRadius, data.position.y + THICKNESS};
        sf::Vector2f maskSize = {outerRadius + 10.0f, LETTER_HEIGHT - (2 * THICKNESS)};
        shapes.push_back(ShapeFactory::CreateRect(maskPos, maskSize, BG_COLOR));
    }

    void DrawV(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const LetterData& data)
    {
        float bottomHeight = LETTER_HEIGHT * 0.6f;
        float bottomWidth = LETTER_WIDTH;
        sf::Vector2f bottomPos = {data.position.x, data.position.y + (LETTER_HEIGHT - bottomHeight)};

        shapes.push_back(ShapeFactory::CreateRect(bottomPos, {bottomWidth, bottomHeight}, data.color));

        sf::Vector2f innerBottomPos = {bottomPos.x + THICKNESS, bottomPos.y + THICKNESS};
        sf::Vector2f innerBottomSize = {bottomWidth - 2 * THICKNESS, bottomHeight - 2 * THICKNESS};
        shapes.push_back(ShapeFactory::CreateRect(innerBottomPos, innerBottomSize, BG_COLOR));

        float topHeight = LETTER_HEIGHT * 0.4f;
        float topWidth = LETTER_WIDTH * 0.8f;

        shapes.push_back(ShapeFactory::CreateRect(data.position, {topWidth, topHeight}, data.color));

        sf::Vector2f innerTopPos = {data.position.x + THICKNESS, data.position.y + THICKNESS};
        sf::Vector2f innerTopSize = {topWidth - 2 * THICKNESS, topHeight - 2 * THICKNESS};
        shapes.push_back(ShapeFactory::CreateRect(innerTopPos, innerTopSize, BG_COLOR));
    }

    void DrawO(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const LetterData& data)
    {
        constexpr float outerRadius = LETTER_HEIGHT / 2.0f;
        constexpr float innerRadius = outerRadius - THICKNESS;

        shapes.push_back(ShapeFactory::CreateCircle(data.position, outerRadius, data.color));

        sf::Vector2f innerPos = {data.position.x + THICKNESS, data.position.y + THICKNESS};
        shapes.push_back(ShapeFactory::CreateCircle(innerPos, innerRadius, BG_COLOR));
    }
} // namespace

LettersView::LettersView(std::shared_ptr<LettersModel> model)
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void LettersView::HandleEvent(const sf::Event&, const sf::RenderWindow&)
{
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
			DrawG(m_shapes, letter);
			break;
		case LetterType::S:
			DrawS(m_shapes, letter);
			break;
		case LetterType::V:
			DrawV(m_shapes, letter);
			break;
		case LetterType::O:
			DrawO(m_shapes, letter);
			break;
		}
	}
}
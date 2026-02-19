#include "PiecesView.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "src/controller/PiecesController.h"
#include "src/model/ChessGrid.h"

namespace
{
constexpr float PIECE_SIZE = ChessGrid::CELL_SIZE - 10.0f;
constexpr float PIECE_OFFSET = (ChessGrid::CELL_SIZE - PIECE_SIZE) / 2.0f;

constexpr sf::Color COLOR_WHITE(255, 255, 255);
constexpr sf::Color COLOR_BLACK(50, 50, 50);
constexpr sf::Color COLOR_BORDER(0, 0, 0);

constexpr uint8_t ALPHA_COLOR = 200u;
} // namespace

PiecesView::PiecesView(
	std::shared_ptr<PiecesModel> model,
	std::shared_ptr<PiecesController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		m_data = model->GetData();
	}
}

void PiecesView::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButton->button == sf::Mouse::Button::Left)
		{
			m_controller->OnMousePressed(mouseButton->position.x, mouseButton->position.y);
		}
	}
	else if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>())
	{
		m_controller->OnMouseMoved(mouseMove->position.x, mouseMove->position.y);
	}
	else if (const auto* mouseButtonRel = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseButtonRel->button == sf::Mouse::Button::Left)
		{
			m_controller->OnMouseReleased(mouseButtonRel->position.x, mouseButtonRel->position.y);
		}
	}
}

void PiecesView::Render(sf::RenderWindow& window) const
{
	sf::RectangleShape shape({PIECE_SIZE, PIECE_SIZE});
	shape.setOutlineThickness(2.0f);
	shape.setOutlineColor(COLOR_BORDER);

	for (const auto& piece : m_data)
	{
		if (!piece.isDragged)
		{
			shape.setPosition({piece.position.x + PIECE_OFFSET, piece.position.y + PIECE_OFFSET});
			shape.setFillColor(piece.color == PieceColor::White ? COLOR_WHITE : COLOR_BLACK);
			window.draw(shape);
		}
	}

	for (const auto& piece : m_data)
	{
		if (piece.isDragged)
		{
			shape.setPosition({piece.position.x + PIECE_OFFSET, piece.position.y + PIECE_OFFSET});
			shape.setFillColor(piece.color == PieceColor::White ? COLOR_WHITE : COLOR_BLACK);

			sf::Color color = shape.getFillColor();
			color.a = ALPHA_COLOR;
			shape.setFillColor(color);

			window.draw(shape);
		}
	}
}

void PiecesView::Update(const ChessData& data, IObservable<ChessData>*)
{
	m_data = data;
}
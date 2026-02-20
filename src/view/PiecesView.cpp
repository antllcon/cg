#include "PiecesView.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "ShapeFactory.h"
#include "src/controller/PiecesController.h"
#include "src/model/ChessGrid.h"

namespace
{
constexpr float PIECE_SIZE = ChessGrid::CELL_SIZE - 10.0f;
constexpr float PIECE_OFFSET = (ChessGrid::CELL_SIZE - PIECE_SIZE) / 2.0f;

constexpr sf::Color COLOR_WHITE(255, 255, 255);
constexpr sf::Color COLOR_BLACK(50, 50, 50);
constexpr sf::Color COLOR_BORDER(0, 0, 0);
constexpr float BORDER_THICKNESS = -1.5f;
constexpr uint8_t ALPHA_COLOR = 200u;

void DrawPawn(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
	shapes.push_back(ShapeFactory::CreateRect(pos, {PIECE_SIZE, PIECE_SIZE}, color, COLOR_BORDER, BORDER_THICKNESS));
}

void DrawRook(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
	shapes.push_back(ShapeFactory::CreateRect({pos.x, pos.y + 8.0f}, {PIECE_SIZE, PIECE_SIZE - 8.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
	shapes.push_back(ShapeFactory::CreateRect({pos.x - 2.0f, pos.y}, {PIECE_SIZE + 4.0f, 10.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
}

void DrawKnight(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
	shapes.push_back(ShapeFactory::CreateRect({pos.x, pos.y + 12.0f}, {PIECE_SIZE, PIECE_SIZE - 12.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
	shapes.push_back(ShapeFactory::CreatePolygon({pos.x - 2.0f, pos.y - 4.0f}, 14.0f, 3, color, COLOR_BORDER, BORDER_THICKNESS));
}

void DrawBishop(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
	shapes.push_back(ShapeFactory::CreateRect({pos.x + 4.0f, pos.y + 12.0f}, {PIECE_SIZE - 8.0f, PIECE_SIZE - 12.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
	shapes.push_back(ShapeFactory::CreatePolygon({pos.x, pos.y - 4.0f}, 12.0f, 4, color, COLOR_BORDER, BORDER_THICKNESS));
}

void DrawQueen(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
	shapes.push_back(ShapeFactory::CreateRect({pos.x, pos.y + 12.0f}, {PIECE_SIZE, PIECE_SIZE - 12.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
	shapes.push_back(ShapeFactory::CreatePolygon({pos.x - 2.0f, pos.y - 4.0f}, 14.0f, 8, color, COLOR_BORDER, BORDER_THICKNESS));
}

void DrawKing(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
	shapes.push_back(ShapeFactory::CreateRect({pos.x, pos.y + 10.0f}, {PIECE_SIZE, PIECE_SIZE - 10.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
	shapes.push_back(ShapeFactory::CreateRect({pos.x + 10.0f, pos.y}, {4.0f, 12.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
	shapes.push_back(ShapeFactory::CreateRect({pos.x + 6.0f, pos.y + 4.0f}, {12.0f, 4.0f}, color, COLOR_BORDER, BORDER_THICKNESS));
}
} // namespace

PiecesView::PiecesView(
	std::shared_ptr<PiecesModel> model,
	std::shared_ptr<PiecesController> controller)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
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
	for (const auto& shape : m_shapes)
	{
		window.draw(*shape);
	}
}

void PiecesView::Update(const ChessData& data, IObservable<ChessData>*)
{
	m_shapes.clear();

	auto addShapesForPiece = [this](const ChessPiece& piece) {
		sf::Vector2f basePos = {piece.position.x + PIECE_OFFSET, piece.position.y + PIECE_OFFSET};
		sf::Color fillColor = piece.color == PieceColor::White ? COLOR_WHITE : COLOR_BLACK;

		if (piece.isDragged)
		{
			fillColor.a = ALPHA_COLOR;
		}

		switch (piece.type)
		{
		case PieceType::Pawn:
			DrawPawn(m_shapes, basePos, fillColor);
			break;
		case PieceType::Rook:
			DrawRook(m_shapes, basePos, fillColor);
			break;
		case PieceType::Knight:
			DrawKnight(m_shapes, basePos, fillColor);
			break;
		case PieceType::Bishop:
			DrawBishop(m_shapes, basePos, fillColor);
			break;
		case PieceType::Queen:
			DrawQueen(m_shapes, basePos, fillColor);
			break;
		case PieceType::King:
			DrawKing(m_shapes, basePos, fillColor);
			break;
		}
	};

	for (const auto& piece : data)
	{
		if (!piece.isDragged)
		{
			addShapesForPiece(piece);
		}
	}

	for (const auto& piece : data)
	{
		if (piece.isDragged)
		{
			addShapesForPiece(piece);
		}
	}
}
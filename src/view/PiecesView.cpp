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

void AssertHasPoints(const std::vector<sf::Vector2f>& points)
{
    if (points.empty())
    {
        throw std::runtime_error("Точки для отрисовки отсутствуют");
    }
}

const std::vector<sf::Vector2f>& GetPawnPoints()
{
    static const std::vector<sf::Vector2f> points =
    {
        {6.0f, 31.0f}, {26.0f, 31.0f}, {19.0f, 19.0f}, {21.5f, 19.0f},
        {22.0f, 15.5f}, {19.0f, 14.0f}, {21.0f, 8.0f}, {16.0f, 5.0f},
        {11.0f, 8.0f}, {12.5f, 14.0f}, {9.0f, 15.5f}, {10.0f, 19.0f},
        {12.5f, 19.0f}
    };
    return points;
}

const std::vector<sf::Vector2f>& GetRookPoints()
{
    static const std::vector<sf::Vector2f> points =
    {
        {5.0f, 26.5f}, {4.0f, 31.5f}, {27.5f, 31.5f}, {26.5f, 26.5f},
        {24.5f, 25.0f}, {22.5f, 12.0f}, {25.0f, 9.5f}, {25.5f, 2.5f},
        {21.5f, 2.0f}, {20.0f, 6.0f}, {19.5f, 6.0f}, {18.5f, 1.5f},
        {13.0f, 1.5f}, {12.0f, 5.5f}, {11.0f, 5.5f}, {10.0f, 2.0f},
        {5.5f, 2.5f}, {6.5f, 9.5f}, {9.0f, 12.0f}, {7.5f, 25.0f}
    };
    return points;
}

const std::vector<sf::Vector2f>& GetKnightPoints()
{
    static const std::vector<sf::Vector2f> points =
    {
        {4.5f, 31.5f}, {28.5f, 31.5f}, {27.0f, 26.5f}, {24.0f, 24.5f},
        {26.0f, 16.0f}, {20.5f, 5.0f}, {14.0f, 3.5f}, {11.0f, 0.5f},
        {10.0f, 3.5f}, {7.5f, 6.0f}, {6.0f, 10.0f}, {2.5f, 15.0f},
        {7.5f, 18.5f}, {9.0f, 15.5f}, {11.0f, 16.0f}, {15.0f, 13.5f},
        {8.5f, 25.0f}, {5.5f, 26.5f}
    };
    return points;
}

const std::vector<sf::Vector2f>& GetBishopPoints()
{
    static const std::vector<sf::Vector2f> points =
    {
        {3.5f, 32.0f}, {28.0f, 32.0f}, {26.0f, 26.0f}, {24.0f, 25.0f},
        {21.5f, 25.0f}, {24.0f, 15.5f}, {19.0f, 6.5f}, {12.0f, 6.5f},
        {7.0f, 16.5f}, {9.5f, 25.0f}, {7.0f, 25.0f}, {5.0f, 26.0f}
    };
    return points;
}

const std::vector<sf::Vector2f>& GetQueenPoints()
{
    static const std::vector<sf::Vector2f> points =
    {
        {5.5f, 26.0f}, {4.0f, 31.5f}, {28.0f, 31.5f}, {26.5f, 26.0f},
        {24.5f, 25.0f}, {30.5f, 7.5f}, {22.5f, 15.0f}, {20.5f, 0.5f},
        {16.0f, 12.5f}, {11.0f, 0.5f}, {9.5f, 15.0f}, {1.5f, 7.5f},
        {8.0f, 25.0f}
    };
    return points;
}

const std::vector<sf::Vector2f>& GetKingPoints()
{
    static const std::vector<sf::Vector2f> points =
    {
        {26.0f, 26.0f}, {27.5f, 31.5f}, {3.5f, 31.5f}, {4.5f, 26.0f},
        {7.0f, 24.5f}, {2.0f, 18.0f}, {3.5f, 9.5f}, {12.5f, 9.0f},
        {13.0f, 7.5f}, {10.5f, 7.5f}, {11.0f, 2.5f}, {13.0f, 2.5f},
        {13.0f, 0.5f}, {17.5f, 0.5f}, {17.5f, 3.0f}, {20.0f, 3.0f},
        {20.0f, 7.5f}, {17.5f, 7.5f}, {18.0f, 9.0f}, {27.5f, 9.5f},
        {29.0f, 18.0f}, {23.5f, 24.5f}
    };
    return points;
}

void AddSvgShape(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const std::vector<sf::Vector2f>& points, const sf::Vector2f& pos, const sf::Color& color)
{
    AssertHasPoints(points);
    shapes.push_back(ShapeFactory::CreatePathShape(pos, points, color, COLOR_BORDER, BORDER_THICKNESS));
}

sf::Vector2f GetPieceBasePosition(const ChessPiece& piece)
{
    if (piece.dragPosition.has_value())
    {
        return piece.dragPosition.value();
    }

    return ChessGrid::GridToPixels(piece.gridPosition);
}

void DrawPawn(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
    AddSvgShape(shapes, GetPawnPoints(), pos, color);
}

void DrawRook(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
    AddSvgShape(shapes, GetRookPoints(), pos, color);
}

void DrawKnight(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
    AddSvgShape(shapes, GetKnightPoints(), pos, color);
}

void DrawBishop(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
    AddSvgShape(shapes, GetBishopPoints(), pos, color);
}

void DrawQueen(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
    AddSvgShape(shapes, GetQueenPoints(), pos, color);
}

void DrawKing(std::vector<std::unique_ptr<sf::Drawable>>& shapes, const sf::Vector2f& pos, const sf::Color& color)
{
    AddSvgShape(shapes, GetKingPoints(), pos, color);
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
			m_controller->OnMousePressed(
				static_cast<float>(mouseButton->position.x),
				static_cast<float>(mouseButton->position.y));
		}
	}
	else if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>())
	{
		m_controller->OnMouseMoved(
			static_cast<float>(mouseMove->position.x),
			static_cast<float>(mouseMove->position.y));
	}
	else if (const auto* mouseButtonRel = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseButtonRel->button == sf::Mouse::Button::Left)
		{
			m_controller->OnMouseReleased(
				static_cast<float>(mouseButtonRel->position.x),
				static_cast<float>(mouseButtonRel->position.y));
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
		sf::Vector2f basePos = GetPieceBasePosition(piece);
		sf::Vector2f pos = {basePos.x + PIECE_OFFSET, basePos.y + PIECE_OFFSET};
		sf::Color fillColor = piece.color == PieceColor::White ? COLOR_WHITE : COLOR_BLACK;

		if (piece.dragPosition.has_value())
		{
			fillColor.a = ALPHA_COLOR;
		}

		switch (piece.type)
		{
		case PieceType::Pawn:
			DrawPawn(m_shapes, pos, fillColor);
			break;
		case PieceType::Rook:
			DrawRook(m_shapes, pos, fillColor);
			break;
		case PieceType::Knight:
			DrawKnight(m_shapes, pos, fillColor);
			break;
		case PieceType::Bishop:
			DrawBishop(m_shapes, pos, fillColor);
			break;
		case PieceType::Queen:
			DrawQueen(m_shapes, pos, fillColor);
			break;
		case PieceType::King:
			DrawKing(m_shapes, pos, fillColor);
			break;
		}
	};

	for (const auto& piece : data)
	{
		if (!piece.dragPosition.has_value())
		{
			addShapesForPiece(piece);
		}
	}

	for (const auto& piece : data)
	{
		if (piece.dragPosition.has_value())
		{
			addShapesForPiece(piece);
		}
	}
}
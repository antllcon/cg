#include "PiecesModel.h"
#include "ChessGrid.h"

#include <stdexcept>

namespace
{
void AssertIsPieceFound(bool isFound)
{
	if (!isFound)
	{
		throw std::invalid_argument("Фигура с указанным идентификатором не найдена");
	}
}

template <typename Container>
auto FindPieceById(Container& data, PieceId id)
{
	return std::find_if(data.begin(), data.end(), [id](const ChessPiece& piece) {
		return piece.id == id;
	});
}
}

PiecesModel::PiecesModel()
{
	AddPiece(PieceType::Rook, PieceColor::Black, {0, 0});
	AddPiece(PieceType::Knight, PieceColor::Black, {1, 0});
	AddPiece(PieceType::Bishop, PieceColor::Black, {2, 0});
	AddPiece(PieceType::Queen, PieceColor::Black, {3, 0});
	AddPiece(PieceType::King, PieceColor::Black, {4, 0});
	AddPiece(PieceType::Bishop, PieceColor::Black, {5, 0});
	AddPiece(PieceType::Knight, PieceColor::Black, {6, 0});
	AddPiece(PieceType::Rook, PieceColor::Black, {7, 0});

	for (int i = 0; i < ChessGrid::BOARD_SIZE; ++i)
	{
		AddPiece(PieceType::Pawn, PieceColor::Black, {i, 1});
		AddPiece(PieceType::Pawn, PieceColor::White, {i, 6});
	}

	AddPiece(PieceType::Rook, PieceColor::White, {0, 7});
	AddPiece(PieceType::Knight, PieceColor::White, {1, 7});
	AddPiece(PieceType::Bishop, PieceColor::White, {2, 7});
	AddPiece(PieceType::Queen, PieceColor::White, {3, 7});
	AddPiece(PieceType::King, PieceColor::White, {4, 7});
	AddPiece(PieceType::Bishop, PieceColor::White, {5, 7});
	AddPiece(PieceType::Knight, PieceColor::White, {6, 7});
	AddPiece(PieceType::Rook, PieceColor::White, {7, 7});
}

const ChessData& PiecesModel::GetData() const
{
	return m_piecesData;
}

PieceColor PiecesModel::GetPieceColor(PieceId id) const
{
	auto it = FindPieceById(m_piecesData, id);
	AssertIsPieceFound(it != m_piecesData.end());
	return it->color;
}

void PiecesModel::AddPiece(PieceType type, PieceColor color, const sf::Vector2i& gridPos)
{
	m_piecesData.push_back({m_nextId++, type, color, gridPos});
	NotifyObservers();
}

void PiecesModel::RemovePiece(PieceId id)
{
	auto it = std::remove_if(m_piecesData.begin(), m_piecesData.end(), [id](const ChessPiece& piece) {
		return piece.id == id;
	});

	if (it != m_piecesData.end())
	{
		m_piecesData.erase(it, m_piecesData.end());
		NotifyObservers();
	}
}

std::optional<PieceId> PiecesModel::GetPieceIdAt(const sf::Vector2i& gridPos) const
{
	for (const auto& piece : m_piecesData)
	{
		if (piece.gridPosition == gridPos && !piece.dragPosition.has_value())
		{
			return piece.id;
		}
	}

	return std::nullopt;
}

void PiecesModel::RemovePieceAt(const sf::Vector2i& gridPos)
{
	auto it = std::remove_if(m_piecesData.begin(), m_piecesData.end(), [&gridPos](const ChessPiece& piece) {
		return piece.gridPosition == gridPos && !piece.dragPosition.has_value();
	});

	if (it != m_piecesData.end())
	{
		m_piecesData.erase(it, m_piecesData.end());
		NotifyObservers();
	}
}

void PiecesModel::StartDrag(PieceId id, const sf::Vector2f& pixelPos)
{
	auto it = FindPieceById(m_piecesData, id);
	AssertIsPieceFound(it != m_piecesData.end());

	it->dragPosition = pixelPos;
	NotifyObservers();
}

void PiecesModel::UpdateDragPosition(PieceId id, const sf::Vector2f& pixelPos)
{
	auto it = FindPieceById(m_piecesData, id);
	AssertIsPieceFound(it != m_piecesData.end());

	if (it->dragPosition.has_value())
	{
		it->dragPosition = pixelPos;
		NotifyObservers();
	}
}

void PiecesModel::DropPiece(PieceId id, const sf::Vector2i& newGridPos)
{
	auto it = FindPieceById(m_piecesData, id);
	AssertIsPieceFound(it != m_piecesData.end());

	it->gridPosition = newGridPos;
	it->dragPosition = std::nullopt;
	NotifyObservers();
}

void PiecesModel::CancelDrag(PieceId id)
{
	auto it = FindPieceById(m_piecesData, id);
	AssertIsPieceFound(it != m_piecesData.end());

	it->dragPosition = std::nullopt;
	NotifyObservers();
}

ChessData PiecesModel::GetChangedData() const
{
	return m_piecesData;
}
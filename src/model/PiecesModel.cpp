#include "PiecesModel.h"

#include "ChessGrid.h"

PiecesModel::PiecesModel()
{
    m_piecesData.push_back({PieceType::Rook, PieceColor::Black, ChessGrid::GridToPixels({0, 0}), {0, 0}, false});
    m_piecesData.push_back({PieceType::Knight, PieceColor::Black, ChessGrid::GridToPixels({1, 0}), {1, 0}, false});
    m_piecesData.push_back({PieceType::Bishop, PieceColor::Black, ChessGrid::GridToPixels({2, 0}), {2, 0}, false});
    m_piecesData.push_back({PieceType::Queen, PieceColor::Black, ChessGrid::GridToPixels({3, 0}), {3, 0}, false});
    m_piecesData.push_back({PieceType::King, PieceColor::Black, ChessGrid::GridToPixels({4, 0}), {4, 0}, false});
    m_piecesData.push_back({PieceType::Bishop, PieceColor::Black, ChessGrid::GridToPixels({5, 0}), {5, 0}, false});
    m_piecesData.push_back({PieceType::Knight, PieceColor::Black, ChessGrid::GridToPixels({6, 0}), {6, 0}, false});
    m_piecesData.push_back({PieceType::Rook, PieceColor::Black, ChessGrid::GridToPixels({7, 0}), {7, 0}, false});

    for (int i = 0; i < ChessGrid::BOARD_SIZE; ++i)
    {
        m_piecesData.push_back({PieceType::Pawn, PieceColor::Black, ChessGrid::GridToPixels({i, 1}), {i, 1}, false});
    }

    for (int i = 0; i < ChessGrid::BOARD_SIZE; ++i)
    {
        m_piecesData.push_back({PieceType::Pawn, PieceColor::White, ChessGrid::GridToPixels({i, 6}), {i, 6}, false});
    }

    m_piecesData.push_back({PieceType::Rook, PieceColor::White, ChessGrid::GridToPixels({0, 7}), {0, 7}, false});
    m_piecesData.push_back({PieceType::Knight, PieceColor::White, ChessGrid::GridToPixels({1, 7}), {1, 7}, false});
    m_piecesData.push_back({PieceType::Bishop, PieceColor::White, ChessGrid::GridToPixels({2, 7}), {2, 7}, false});
    m_piecesData.push_back({PieceType::Queen, PieceColor::White, ChessGrid::GridToPixels({3, 7}), {3, 7}, false});
    m_piecesData.push_back({PieceType::King, PieceColor::White, ChessGrid::GridToPixels({4, 7}), {4, 7}, false});
    m_piecesData.push_back({PieceType::Bishop, PieceColor::White, ChessGrid::GridToPixels({5, 7}), {5, 7}, false});
    m_piecesData.push_back({PieceType::Knight, PieceColor::White, ChessGrid::GridToPixels({6, 7}), {6, 7}, false});
    m_piecesData.push_back({PieceType::Rook, PieceColor::White, ChessGrid::GridToPixels({7, 7}), {7, 7}, false});
}

ChessData& PiecesModel::GetData()
{
	return m_piecesData;
}

const ChessData& PiecesModel::GetData() const
{
	return m_piecesData;
}

void PiecesModel::UpdatePiecePosition(size_t index, const sf::Vector2f& position)
{
	if (index < m_piecesData.size())
	{
		m_piecesData[index].position = position;
		NotifyObservers();
	}
}

void PiecesModel::SnapPieceToGrid(size_t index, const sf::Vector2i& gridPos, const sf::Vector2f& visualPos)
{
	if (index < m_piecesData.size())
	{
		m_piecesData[index].gridPosition = gridPos;
		m_piecesData[index].position = visualPos;
		NotifyObservers();
	}
}

void PiecesModel::SetIsDragged(size_t index, bool isDragged)
{
	if (index < m_piecesData.size())
	{
		m_piecesData[index].isDragged = isDragged;
		NotifyObservers();
	}
}

std::optional<size_t> PiecesModel::GetPieceIndexAt(const sf::Vector2i& gridPos) const
{
	for (size_t i = 0; i < m_piecesData.size(); ++i)
	{
		if (m_piecesData[i].gridPosition == gridPos && !m_piecesData[i].isDragged)
		{
			return i;
		}
	}
	return std::nullopt;
}

void PiecesModel::RemovePieceAt(const sf::Vector2i& gridPos)
{
	auto it = std::remove_if(m_piecesData.begin(), m_piecesData.end(), [&](const ChessPiece& p) {
		return p.gridPosition == gridPos && !p.isDragged;
	});

	if (it != m_piecesData.end())
	{
		m_piecesData.erase(it, m_piecesData.end());
		NotifyObservers();
	}
}

ChessData PiecesModel::GetChangedData() const
{
	return m_piecesData;
}
#include "PiecesModel.h"

#include "ChessGrid.h"

PiecesModel::PiecesModel()
{
	for (int i = 0; i < 8; ++i)
	{
		sf::Vector2i gridPos{i, 6};
		sf::Vector2f pixelPos = ChessGrid::GridToPixels(gridPos);
		m_piecesData.push_back({PieceType::Pawn, PieceColor::White, pixelPos, gridPos, false});
	}

	for (int i = 0; i < 8; ++i)
	{
		sf::Vector2i gridPos{i, 1};
		sf::Vector2f pixelPos = ChessGrid::GridToPixels(gridPos);
		m_piecesData.push_back({PieceType::Pawn, PieceColor::Black, pixelPos, gridPos, false});
	}
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
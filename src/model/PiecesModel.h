#pragma once
#include "../system/Observer.h"
#include "SFML/System/Vector2.hpp"

#include <optional>
#include <vector>

enum class PieceType
{
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King
};

enum class PieceColor
{
	White,
	Black
};

struct ChessPiece
{
	PieceType type;
	PieceColor color;
	sf::Vector2f position;
	sf::Vector2i gridPosition;
	bool isDragged;
};

using ChessData = std::vector<ChessPiece>;

class PiecesModel final : public CObservable<ChessData>
{
public:
	PiecesModel();

	ChessData& GetData();
	const ChessData& GetData() const;

	void UpdatePiecePosition(size_t index, const sf::Vector2f& position);
	void SnapPieceToGrid(size_t index, const sf::Vector2i& gridPos, const sf::Vector2f& visualPos);
	void SetIsDragged(size_t index, bool isDragged);

	std::optional<size_t> GetPieceIndexAt(const sf::Vector2i& gridPos) const;
	void RemovePieceAt(const sf::Vector2i& gridPos);

protected:
	ChessData GetChangedData() const override;

private:
	ChessData m_piecesData;
};
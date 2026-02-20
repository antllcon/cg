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

using PieceId = uint32_t;

struct ChessPiece
{
	PieceId id;
	PieceType type;
	PieceColor color;
	sf::Vector2i gridPosition;
	std::optional<sf::Vector2f> dragPosition; // не в модели
};

using ChessData = std::vector<ChessPiece>;

// Убрать Drag
class PiecesModel final : public CObservable<ChessData>
{
public:
	PiecesModel();

	const ChessData& GetData() const;
	PieceColor GetPieceColor(PieceId id) const;

	void AddPiece(PieceType type, PieceColor color, const sf::Vector2i& gridPos);
	void RemovePiece(PieceId id);

	std::optional<PieceId> GetPieceIdAt(const sf::Vector2i& gridPos) const;
	void RemovePieceAt(const sf::Vector2i& gridPos);

	void StartDrag(PieceId id, const sf::Vector2f& pixelPos);
	void UpdateDragPosition(PieceId id, const sf::Vector2f& pixelPos);
	void DropPiece(PieceId id, const sf::Vector2i& newGridPos);
	void CancelDrag(PieceId id);


protected:
	ChessData GetChangedData() const override;

private:
	ChessData m_piecesData;
	PieceId m_nextId;
};
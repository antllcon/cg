#include "PiecesController.h"
#include "src/model/ChessGrid.h"
#include "src/model/PiecesModel.h"

PiecesController::PiecesController(std::shared_ptr<PiecesModel> model)
	: m_model(std::move(model))
{
}

void PiecesController::Update(float)
{
}

void PiecesController::OnMousePressed(float x, float y)
{
	const sf::Vector2i gridPos = ChessGrid::PixelsToGrid(x, y);
	const auto idOpt = m_model->GetPieceIdAt(gridPos);

	if (idOpt.has_value())
	{
		m_draggedPieceId = *idOpt;

		const sf::Vector2f pieceBasePixel = ChessGrid::GridToPixels(gridPos);
		m_dragOffsetX = pieceBasePixel.x - x;
		m_dragOffsetY = pieceBasePixel.y - y;

		const sf::Vector2f startDragPos = {x + m_dragOffsetX, y + m_dragOffsetY};
		m_model->StartDrag(*m_draggedPieceId, startDragPos);
	}
}

void PiecesController::OnMouseMoved(float x, float y)
{
	if (m_draggedPieceId.has_value())
	{
		const sf::Vector2f newDragPos = {x + m_dragOffsetX, y + m_dragOffsetY};
		m_model->UpdateDragPosition(*m_draggedPieceId, newDragPos);
	}
}

void PiecesController::OnMouseReleased(float x, float y)
{
	if (!m_draggedPieceId.has_value())
	{
		return;
	}

	const PieceId id = *m_draggedPieceId;
	m_draggedPieceId.reset();

	const sf::Vector2i targetGridPos = ChessGrid::PixelsToGrid(x, y);

	if (!ChessGrid::IsInsideBoard(targetGridPos))
	{
		m_model->CancelDrag(id);
		return;
	}

	const auto targetIdOpt = m_model->GetPieceIdAt(targetGridPos);

	if (targetIdOpt.has_value())
	{
		const PieceColor draggedColor = m_model->GetPieceColor(id);
		const PieceColor targetColor = m_model->GetPieceColor(*targetIdOpt);

		if (targetColor == draggedColor)
		{
			m_model->CancelDrag(id);
			return;
		}

		m_model->RemovePieceAt(targetGridPos);
	}

	m_model->DropPiece(id, targetGridPos);
}
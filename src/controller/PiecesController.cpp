#include "PiecesController.h"
#include "src/model/ChessGrid.h"
#include "src/model/PiecesModel.h"

namespace
{
constexpr float HALF_CELL = ChessGrid::CELL_SIZE / 2.0f;
}

PiecesController::PiecesController(std::shared_ptr<PiecesModel> model)
	: m_model(std::move(model))
{
}

void PiecesController::Update(float dt)
{
	(void)dt;
}

void PiecesController::OnMousePressed(int x, int y)
{
	sf::Vector2i gridPos = ChessGrid::PixelsToGrid(x, y);

	auto indexOpt = m_model->GetPieceIndexAt(gridPos);

	if (indexOpt.has_value())
	{
		m_draggedPieceIndex = indexOpt.value();

		const auto& pieces = m_model->GetData();
		sf::Vector2f currentPos = pieces[*m_draggedPieceIndex].position;

		m_dragOffsetX = currentPos.x - static_cast<float>(x);
		m_dragOffsetY = currentPos.y - static_cast<float>(y);

		m_model->SetIsDragged(*m_draggedPieceIndex, true);
	}
}

void PiecesController::OnMouseMoved(int x, int y)
{
	if (m_draggedPieceIndex.has_value())
	{
		float posX = static_cast<float>(x) - HALF_CELL;
		float posY = static_cast<float>(y) + m_dragOffsetY;

		m_model->UpdatePiecePosition(*m_draggedPieceIndex, {posX, posY});
	}
}

void PiecesController::OnMouseReleased(int x, int y)
{
	if (!m_draggedPieceIndex.has_value())
	{
		return;
	}

	size_t index = *m_draggedPieceIndex;

	sf::Vector2i targetGridPos = ChessGrid::PixelsToGrid(x, y);

	bool moveValid = false;

	if (ChessGrid::IsInsideBoard(targetGridPos))
	{
		const auto& pieces = m_model->GetData();
		const auto& currentPiece = pieces[index];

		auto otherPieceIndex = m_model->GetPieceIndexAt(targetGridPos);

		if (otherPieceIndex.has_value())
		{
			const auto& targetPiece = pieces[*otherPieceIndex];

			if (targetPiece.color != currentPiece.color)
			{
				m_model->RemovePieceAt(targetGridPos);

				// Так как мы удалили фигуру, индексы могли сместиться.
				// Но RemovePieceAt использует erase-remove идиому.
				// Если удаляемая фигура была ПЕРЕД текущей, индекс текущей съедет.
				// Это сложный момент при использовании vector.
				// Для простоты реализации пересчитаем индекс или сделаем допущение.
				// Лучше всего получить актуальный индекс снова, но в рамках задачи (простой MVC)
				// допустим, что RemovePieceAt корректно обрабатывает данные,
				// но нам нужно обновить наш index, если вектор изменился.

				// В текущей реализации PiecesModel::RemovePieceAt может инвалидировать index.
				// Безопаснее найти наш dragged piece заново.
				// Но так как dragged piece имеет флаг isDragged=true, его найти легко.

				auto& data = m_model->GetData();
				for (size_t i = 0; i < data.size(); ++i)
				{
					if (data[i].isDragged)
					{
						index = i;
						break;
					}
				}

				moveValid = true;
			}
			else
			{
				moveValid = false;
			}
		}
		else
		{
			moveValid = true;
		}
	}

	if (moveValid)
	{
		sf::Vector2f snapPos = ChessGrid::GridToPixels(targetGridPos);
		m_model->SnapPieceToGrid(index, targetGridPos, snapPos);
	}
	else
	{
		ResetToOriginalPosition(index);
	}

	m_model->SetIsDragged(index, false);
	m_draggedPieceIndex.reset();
}

void PiecesController::ResetToOriginalPosition(size_t index)
{
	const auto& piece = m_model->GetData()[index];
	sf::Vector2f originalPixelPos = ChessGrid::GridToPixels(piece.gridPosition);
	m_model->UpdatePiecePosition(index, originalPixelPos);
}
#pragma once
#include "IController.h"
#include <memory>
#include <optional>

class PiecesModel;

class PiecesController final : public IController
{
public:
	explicit PiecesController(std::shared_ptr<PiecesModel> model);

	void Update(float) override;

	void OnMousePressed(int x, int y);
	void OnMouseMoved(int x, int y);
	void OnMouseReleased(int x, int y);

private:
	void ResetToOriginalPosition(size_t index);

	std::shared_ptr<PiecesModel> m_model;
	std::optional<size_t> m_draggedPieceIndex;

	float m_dragOffsetX = 0.0f;
	float m_dragOffsetY = 0.0f;
};
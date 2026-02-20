#pragma once
#include "IController.h"
#include "src/model/PiecesModel.h"
#include <memory>
#include <optional>

class PiecesController final : public IController
{
public:
	explicit PiecesController(std::shared_ptr<PiecesModel> model);

	void Update(float) override;

	void OnMousePressed(float x, float y);
	void OnMouseMoved(float x, float y);
	void OnMouseReleased(float x, float y);

private:
	std::shared_ptr<PiecesModel> m_model;
	std::optional<PieceId> m_draggedPieceId;

	float m_dragOffsetX = 0.0f;
	float m_dragOffsetY = 0.0f;
};
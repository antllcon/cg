#include "Model.h"

#include <cmath>
#include <stdexcept>

namespace
{
constexpr float ZOOM_MULTIPLIER = 1.25f;

void AssertDeltaValid(float delta)
{
	if (std::isnan(delta) || std::isinf(delta))
	{
		throw std::invalid_argument("Дельта изменения должна быть конечным числом");
	}
}

void AssertZoomValid(float zoom)
{
	if (zoom <= 0.0f || std::isnan(zoom) || std::isinf(zoom))
	{
		throw std::invalid_argument("Значение зума должно быть строго положительным конечным числом");
	}
}
} // namespace

void Model::NextFractalType()
{
	++m_data.type;
	ResetView();
}

void Model::MoveCamera(float dx, float dy)
{
	AssertDeltaValid(dx);
	AssertDeltaValid(dy);

	m_data.offsetX += dx / m_data.zoom;
	m_data.offsetY += dy / m_data.zoom;

	NotifyObservers();
}

void Model::ZoomCamera(float delta)
{
	AssertDeltaValid(delta);

	float newZoom = m_data.zoom * std::pow(ZOOM_MULTIPLIER, delta);

	AssertZoomValid(newZoom);
	m_data.zoom = newZoom;

	NotifyObservers();
}

void Model::ResetView()
{
	m_data.offsetX = FractalDefaults::OFFSET_X;
	m_data.offsetY = FractalDefaults::OFFSET_Y;
	m_data.zoom = FractalDefaults::ZOOM;

	NotifyObservers();
}

ModelData Model::GetState() const
{
	return m_data;
}

ModelData Model::GetChangedData() const
{
	return m_data;
}
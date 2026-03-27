#include "FpsModel.h"

void FpsModel::Update(float dt)
{
	m_timer += dt;
	m_frameCount++;

	if (m_timer >= 1.0f)
	{
		m_data.fps = m_frameCount;
		m_timer -= 1.0f;
		m_frameCount = 0;
		NotifyObservers();
	}
}

const FpsData& FpsModel::GetData() const
{
	return m_data;
}

FpsData FpsModel::GetChangedData() const
{
	return m_data;
}
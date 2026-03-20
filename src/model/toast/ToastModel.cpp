#include "ToastModel.h"

void ToastModel::Show(const std::string& message)
{
	m_data.message = message;
	m_data.isVisible = true;
	NotifyObservers();
}

void ToastModel::Hide()
{
	if (m_data.isVisible)
	{
		m_data.isVisible = false;
		NotifyObservers();
	}
}

void ToastModel::UpdateAnimation(float alpha, float offsetY)
{
	m_data.alpha = alpha;
	m_data.offsetY = offsetY;
	NotifyObservers();
}

const ToastData& ToastModel::GetData() const
{
	return m_data;
}

ToastData ToastModel::GetChangedData() const
{
	return m_data;
}
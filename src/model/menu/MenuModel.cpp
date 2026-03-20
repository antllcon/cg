#include "MenuModel.h"

MenuModel::MenuModel()
{
}

void MenuModel::SetInteractable(bool state)
{
	m_data.m_isInteractable = state;
	NotifyObservers();
}

bool MenuModel::IsInteractable() const noexcept
{
	return m_data.m_isInteractable;
}
#pragma once
#include "src/system/Observer.h"

struct MenuData
{
	bool m_isInteractable{false};
};

class MenuModel final : public IObservable<MenuData>
{
public:
	MenuModel();

	void SetInteractable(bool state);
	[[nodiscard]] bool IsInteractable() const noexcept;

private:
	MenuData m_data;
};
#pragma once

#include "src/system/Observer.h"
#include <string>

struct ToastData
{
	std::string message;
	bool isVisible{false};
	float alpha{0.0f};
	float offsetY{0.0f};
};

class ToastModel final : public CObservable<ToastData>
{
public:
	ToastModel() = default;

	void Show(const std::string& message);
	void Hide();
	void UpdateAnimation(float alpha, float offsetY);

	const ToastData& GetData() const;

protected:
	ToastData GetChangedData() const override;

private:
	ToastData m_data;
};
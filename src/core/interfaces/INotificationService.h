#pragma once
#include <string>

class INotificationService
{
public:
	virtual ~INotificationService() = default;
	virtual void ShowMessage(const std::string& message) = 0;
	virtual void ShowError(const std::string& message) = 0;
};
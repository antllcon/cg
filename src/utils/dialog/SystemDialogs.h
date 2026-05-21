#pragma once

#include <optional>
#include <string>
#include <vector>

namespace SystemDialogs
{
std::optional<std::string> OpenFile(
	const std::string& title = "Выберите файл",
	const std::vector<std::string>& filters = {"*"});

std::optional<std::string> SaveFile(
	const std::string& title = "Сохранить как...",
	const std::vector<std::string>& filters = {"*"});
} // namespace SystemDialogs
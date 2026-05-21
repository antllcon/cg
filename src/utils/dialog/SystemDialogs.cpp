#include "SystemDialogs.h"
#include "libs/stb/stb_filedialogs.h"
#include <stdexcept>

namespace
{
void AssertDialogsAvailable()
{
	if (!pfd::settings::available())
	{
		throw std::runtime_error("Файловые диалоги не поддерживаются на данной платформе");
	}
}
} // namespace

namespace SystemDialogs
{

std::optional<std::string> OpenFile(const std::string& title, const std::vector<std::string>& filters)
{
	AssertDialogsAvailable();

	pfd::open_file dialog(title, "", filters, pfd::opt::none);
	const std::vector<std::string> result = dialog.result();

	if (result.empty())
	{
		return std::nullopt;
	}

	return result.front();
}

std::optional<std::string> SaveFile(const std::string& title, const std::vector<std::string>& filters)
{
	AssertDialogsAvailable();

	pfd::save_file dialog(title, "", filters, pfd::opt::force_overwrite);
	const std::string result = dialog.result();

	if (result.empty())
	{
		return std::nullopt;
	}

	return result;
}

} // namespace SystemDialogs
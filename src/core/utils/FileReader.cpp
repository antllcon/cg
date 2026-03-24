#include "FileReader.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace
{
void AssertIsFileExists(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Файл не найден по указанному пути: " + path.string());
	}
}

void AssertIsFileOpened(bool isOpen, const std::filesystem::path& path)
{
	if (!isOpen)
	{
		throw std::runtime_error("Не удалось открыть файл для чтения: " + path.string());
	}
}
} // namespace

namespace FileReader
{
std::string ReadFileToString(const std::filesystem::path& path)
{
	AssertIsFileExists(path);

	std::ifstream file(path, std::ios::in | std::ios::binary);
	AssertIsFileOpened(file.is_open(), path);

	return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
} // namespace FileReader
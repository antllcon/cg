#pragma once
#include <filesystem>
#include <string>

namespace FileReader
{
std::string ReadFileToString(const std::filesystem::path& path);
}
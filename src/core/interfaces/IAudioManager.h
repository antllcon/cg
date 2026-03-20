#pragma once
#include <filesystem>

class IAudioManager
{
public:
	virtual ~IAudioManager() = default;
	virtual void PlaySoundFile(const std::filesystem::path& filePath) = 0;
};
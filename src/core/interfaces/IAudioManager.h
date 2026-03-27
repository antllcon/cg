#pragma once
#include <filesystem>

class IAudioManager
{
public:
	virtual ~IAudioManager() = default;
	virtual void PreloadSound(const std::filesystem::path& filePath) = 0;
	virtual void PlaySoundFile(const std::filesystem::path& filePath) = 0;

	virtual void PlayMusic(const std::filesystem::path& filePath) = 0;
	virtual void SetMusicPaused(bool isPaused) = 0;
};
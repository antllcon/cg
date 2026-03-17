#pragma once
#include <string>

class IAudioManager
{
public:
	virtual ~IAudioManager() = default;
	virtual void PlaySoundFile(const std::string& filePath) = 0;
};
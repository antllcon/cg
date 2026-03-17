#pragma once
#include "src/core/interfaces/IAudioManager.h"
#include <map>
#include <string>

struct ma_engine;
struct ma_sound;

class AudioManager final : public IAudioManager
{
public:
	AudioManager();
	~AudioManager() override;

	void PlaySoundFile(const std::string& filePath) override;

private:
	ma_engine* m_engine;
	std::map<std::string, ma_sound*> m_sounds;
};
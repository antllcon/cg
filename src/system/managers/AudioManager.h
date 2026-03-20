#pragma once
#include "src/core/interfaces/IAudioManager.h"
#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

struct ma_engine;
struct ma_sound;

class AudioManager : public IAudioManager
{
public:
	AudioManager();
	~AudioManager() override;

	void PreloadSound(const std::filesystem::path& filePath);
	void PlaySoundFile(const std::filesystem::path& filePath) override;

private:
	static constexpr size_t MAX_VOICES = 4;

	struct EngineDeleter
	{
		void operator()(ma_engine* engine) const;
	};

	struct SoundDeleter
	{
		void operator()(ma_sound* sound) const;
	};

	std::unique_ptr<ma_engine, EngineDeleter> m_engine;
	std::unordered_map<std::string, std::unique_ptr<ma_sound, SoundDeleter>> m_soundCache;
	std::array<std::unique_ptr<ma_sound, SoundDeleter>, MAX_VOICES> m_voices;
	size_t m_voiceIndex;
};
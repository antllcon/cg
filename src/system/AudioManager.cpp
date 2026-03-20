#include "AudioManager.h"
#include <miniaudio.h>
#include <stdexcept>

namespace
{
void AssertIsAudioInitialized(int result)
{
	if (result != MA_SUCCESS)
	{
		throw std::runtime_error("Не удалось инициализировать аудио движок");
	}
}

void AssertIsSoundLoaded(int result)
{
	if (result != MA_SUCCESS)
	{
		throw std::runtime_error("Не удалось загрузить звуковой файл");
	}
}

void AssertIsFileExists(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Файл звука не найден по указанному пути");
	}
}
} // namespace

void AudioManager::EngineDeleter::operator()(ma_engine* engine) const
{
	if (engine)
	{
		ma_engine_uninit(engine);
		delete engine;
	}
}

void AudioManager::SoundDeleter::operator()(ma_sound* sound) const
{
	if (sound)
	{
		ma_sound_uninit(sound);
		delete sound;
	}
}

AudioManager::AudioManager()
	: m_engine(new ma_engine)
	, m_voiceIndex(0)
{
	AssertIsAudioInitialized(ma_engine_init(nullptr, m_engine.get()));

	for (size_t i = 0; i < MAX_VOICES; ++i)
	{
		m_voices[i].reset(new ma_sound);
	}
}

AudioManager::~AudioManager() = default;

void AudioManager::PreloadSound(const std::filesystem::path& filePath)
{
	std::string pathKey = filePath.string();

	if (m_soundCache.contains(pathKey))
	{
		return;
	}

	AssertIsFileExists(filePath);

	auto newSound = std::unique_ptr<ma_sound, SoundDeleter>(new ma_sound);

	AssertIsSoundLoaded(ma_sound_init_from_file(
		m_engine.get(),
		pathKey.c_str(),
		MA_SOUND_FLAG_DECODE,
		nullptr,
		nullptr,
		newSound.get()));

	m_soundCache[pathKey] = std::move(newSound);
}

void AudioManager::PlaySoundFile(const std::filesystem::path& filePath)
{
	PreloadSound(filePath);

	std::string pathKey = filePath.string();
	ma_sound* sourceSound = m_soundCache[pathKey].get();
	ma_sound* voice = m_voices[m_voiceIndex].get();

	ma_sound_stop(voice);
	ma_sound_uninit(voice);

	AssertIsSoundLoaded(ma_sound_init_copy(
		m_engine.get(),
		sourceSound,
		0,
		nullptr,
		voice));

	ma_sound_start(voice);

	m_voiceIndex = (m_voiceIndex + 1) % MAX_VOICES;
}
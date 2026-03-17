#include "AudioManager.h"
#include <stdexcept>

#define MINIAUDIO_IMPLEMENTATION
#pragma warning(push, 0)
#include <miniaudio.h>
#pragma warning(pop)

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
} // namespace

AudioManager::AudioManager()
	: m_engine(new ma_engine)
{
	AssertIsAudioInitialized(ma_engine_init(nullptr, m_engine));
}

AudioManager::~AudioManager()
{
	for (auto& [path, sound] : m_sounds)
	{
		ma_sound_uninit(sound);
		delete sound;
	}

	ma_engine_uninit(m_engine);
	delete m_engine;
}

void AudioManager::PlaySoundFile(const std::string& filePath)
{
	if (!m_sounds.contains(filePath))
	{
		ma_sound* newSound = new ma_sound;
		AssertIsSoundLoaded(ma_sound_init_from_file(
			m_engine,
			filePath.c_str(),
			0,
			nullptr,
			nullptr,
			newSound));

		m_sounds[filePath] = newSound;
	}

	ma_sound* sound = m_sounds[filePath];

	ma_sound_stop(sound);
	ma_sound_seek_to_pcm_frame(sound, 0);
	ma_sound_start(sound);
}
#include "AudioSource.h"
#include <filesystem>

namespace taskhub {

	AudioSource::AudioSource()
		: m_Sound(std::make_unique<ma_sound>())
	{
	}

	AudioSource::~AudioSource() {
		if (m_IsLoaded)
			ma_sound_uninit(m_Sound.get());
	}

	void AudioSource::Load(AudioFile& audioFile) {

		Unload();

		ma_result result;
		result = ma_sound_init_from_data_source(HubAudioEngine::GetInstance()->GetEngineHandle(), audioFile.GetDecoder(), MA_SOUND_FLAG_ASYNC, nullptr, m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize sound");

		m_IsLoaded = true;
	}

	void AudioSource::Unload() {

		if (m_IsLoaded) {
			ma_sound_uninit(m_Sound.get());
			m_IsLoaded = false;
		}
	}

	void AudioSource::Play() {

		ma_result result;
		result = ma_sound_start(m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to start sound");
	}

	void AudioSource::Pause() {

		ma_result result;
		
		result = ma_sound_stop(m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to stop sound");
	}

	void AudioSource::Seek(float seconds) {

		ma_result result;
		result = ma_sound_seek_to_pcm_frame(m_Sound.get(), static_cast<uint64_t>(seconds * HubAudioEngine::GetInstance()->GetSampleRate()));
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to seek to pcm frame")
	}

	void AudioSource::SetLooping(bool isLooping) {
		ma_sound_set_looping(m_Sound.get(), isLooping);
	}

	float AudioSource::GetCursorPosition() const {

		ma_result result;
		float cursorPos;
		result = ma_sound_get_cursor_in_seconds(m_Sound.get(), &cursorPos);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get cursor position");

		return cursorPos;
	}

	bool AudioSource::IsPlaying() const {
		return ma_sound_is_playing(m_Sound.get());
	}

	bool AudioSource::IsFinished() const {
		return ma_sound_at_end(m_Sound.get());
	}

	bool AudioSource::IsLooping() const {
		return ma_sound_is_looping(m_Sound.get());
	}
}
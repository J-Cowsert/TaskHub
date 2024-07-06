#include "AudioSource.h"
#include "AudioEngine.h"
#include "Core/Assert.h"

namespace taskhub {

	AudioSource::AudioSource()
		: m_SoundHandle(std::make_unique<ma_sound>())
	{
	}

	AudioSource::~AudioSource() {
		Unload(); 
	}

	void AudioSource::Load(AudioFile& audioFile) {

		Unload();

		ma_result result;
		result = ma_sound_init_from_file(AudioEngine::GetInstance()->GetEngineHandle(), audioFile.GetFilepath().c_str(), MA_SOUND_FLAG_ASYNC, nullptr, nullptr, m_SoundHandle.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize sound");
		m_IsLoaded = true;
	}

	void AudioSource::Unload() {

		if (m_IsLoaded) {
			ma_sound_uninit(m_SoundHandle.get());
			m_IsLoaded = false;
		}
	}

	void AudioSource::Play() {

		HUB_CORE_ASSERT(m_IsLoaded, "Sound is not loaded");
		ma_result result;
		result = ma_sound_start(m_SoundHandle.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to start sound");
	}
	void AudioSource::Pause() {

		HUB_CORE_ASSERT(m_IsLoaded, "Sound is not loaded");
		ma_result result;
		result = ma_sound_stop(m_SoundHandle.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to stop sound");
	}

	void AudioSource::SetVolume(float volume) {
		ma_sound_set_volume(m_SoundHandle.get(), volume);
	}

	void AudioSource::SetCursorPosition(float seconds) {

		HUB_CORE_ASSERT(m_IsLoaded, "Sound is not loaded");
		ma_result result;
		result = ma_sound_seek_to_pcm_frame(m_SoundHandle.get(), static_cast<uint64_t>(seconds * AudioEngine::GetInstance()->GetSampleRate()));
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to seek to pcm frame")
	}

	void AudioSource::SetLooping(bool isLooping) {
		ma_sound_set_looping(m_SoundHandle.get(), isLooping);
	}

	float AudioSource::GetCursorPosition() const {

		HUB_CORE_ASSERT(m_IsLoaded, "Sound is not loaded");
		ma_result result;
		float cursorPos;
		result = ma_sound_get_cursor_in_seconds(m_SoundHandle.get(), &cursorPos);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get cursor position");

		return cursorPos;
	}

	float AudioSource::GetVolume() const {
		return ma_sound_get_volume(m_SoundHandle.get());
	}

	bool AudioSource::IsPlaying() const {
		return ma_sound_is_playing(m_SoundHandle.get());
	}

	bool AudioSource::IsFinished() const {
		return ma_sound_at_end(m_SoundHandle.get());
	}

	bool AudioSource::IsLooping() const {
		return ma_sound_is_looping(m_SoundHandle.get());
	}
}
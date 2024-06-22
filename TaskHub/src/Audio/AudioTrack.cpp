#include "AudioTrack.h"
#include <filesystem>

namespace taskhub {

	AudioTrack::AudioTrack(std::shared_ptr<AudioEngine> engineHandle, const std::string& filepath)
		: m_EngineHandle(engineHandle), m_Filepath(filepath), m_Sound(std::make_unique<ma_sound>()), m_Duration(std::chrono::duration<float>(0.0f))
	{
		HUB_CORE_ASSERT(std::filesystem::exists(m_Filepath), "Filepath does not exist: " + m_Filepath);
	}

	AudioTrack::~AudioTrack() {
		if (m_IsLoaded)
			ma_sound_uninit(m_Sound.get());
	}

	void AudioTrack::Load() {

		if (m_IsLoaded)
			return;

		ma_result result;
		result = ma_sound_init_from_file((ma_engine*)m_EngineHandle->GetContext(), m_Filepath.c_str(), 0, nullptr, nullptr, m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize sound");

		// Calculate and store the duration
		ma_uint64 frameCount;
		result = ma_sound_get_length_in_pcm_frames(m_Sound.get(), &frameCount);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get length of sound");

		ma_uint32 sampleRate = m_EngineHandle->GetSampleRate();
		m_Duration = std::chrono::duration<float>(static_cast<float>(frameCount) / sampleRate);

		m_IsLoaded = true;
	}

	void AudioTrack::Unload() {

		if (m_IsLoaded) {
			ma_sound_uninit(m_Sound.get());
			m_IsLoaded = false;
		}
	}

	void AudioTrack::Play() {

		ma_result result;
		result = ma_sound_start(m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to start sound");
	}

	void AudioTrack::Pause() {

		ma_result result;
		result = ma_sound_stop(m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to stop sound");
	}

	void AudioTrack::Seek(float seconds) {

		ma_result result;
		result = ma_sound_seek_to_pcm_frame(m_Sound.get(), static_cast<uint64_t>(seconds * m_EngineHandle->GetSampleRate()));
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to seek to pcm frame")
	}

	void AudioTrack::SetLooping(bool isLooping) {
		ma_sound_set_looping(m_Sound.get(), isLooping);
	}

	float AudioTrack::GetCursorPosition() const {

		ma_result result;
		float cursorPos;
		result = ma_sound_get_cursor_in_seconds(m_Sound.get(), &cursorPos);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get cursor position");

		return cursorPos;
	}

	bool AudioTrack::IsPlaying() const {
		return ma_sound_is_playing(m_Sound.get());
	}

	bool AudioTrack::IsFinished() const {
		return ma_sound_at_end(m_Sound.get());
	}

	bool AudioTrack::IsLooping() const {
		return ma_sound_is_looping(m_Sound.get());
	}
}
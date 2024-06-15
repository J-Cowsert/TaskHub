#include "AudioFile.h"

namespace taskhub {

	AudioFile::AudioFile(std::shared_ptr<AudioEngine> engine, const std::string& filepath)
		: m_Engine(engine), m_Filepath(filepath), m_Sound(std::make_unique<ma_sound>())
	{
		Load();
	}

	AudioFile::~AudioFile() {
		ma_sound_uninit(m_Sound.get());
	}

	void AudioFile::Load() {

		ma_result result;
		result = ma_sound_init_from_file((ma_engine*)m_Engine->GetContext(), m_Filepath.c_str(), 0, nullptr, nullptr, m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize sound");

		// Calculate and store the duration
		ma_uint64 frameCount;
		result = ma_sound_get_length_in_pcm_frames(m_Sound.get(), &frameCount);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get length of sound");

		ma_uint32 sampleRate = m_Engine->GetSampleRate();
		m_Duration = std::chrono::duration<float>(static_cast<float>(frameCount) / sampleRate);
	}

	void AudioFile::Play() {

		ma_result result;
		result = ma_sound_start(m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to start sound");
	}

	void AudioFile::Pause() {

		ma_result result;
		result = ma_sound_stop(m_Sound.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to stop sound");
	}

	void AudioFile::Seek(float seconds) {

		ma_result result;
		result = ma_sound_seek_to_pcm_frame(m_Sound.get(), static_cast<uint64_t>(seconds * m_Engine->GetSampleRate()));
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to seek to pcm frame")
	}

	void AudioFile::SetLooping(bool isLooping) {
		ma_sound_set_looping(m_Sound.get(), isLooping);
	}

	float AudioFile::GetCursorPosition() const {

		ma_result result;
		float cursorPos;
		result = ma_sound_get_cursor_in_seconds(m_Sound.get(), &cursorPos);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get cursor position");

		return cursorPos;
	}

	bool AudioFile::IsPlaying() const {
		return ma_sound_is_playing(m_Sound.get());
	}

	bool AudioFile::IsFinished() const {
		return ma_sound_at_end(m_Sound.get());
	}

	bool AudioFile::IsLooping() const {
		return ma_sound_is_looping(m_Sound.get());
	}
}
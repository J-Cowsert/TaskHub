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
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to initaialize {0}", (char*)result);
		}

		// Calculate and store the duration
		ma_uint64 frameCount;
		result = ma_sound_get_length_in_pcm_frames(m_Sound.get(), &frameCount);
		HUB_ASSERT(result == MA_SUCCESS, "Failed to get length of sound");

		ma_uint32 sampleRate = m_Engine->GetSampleRate();
		m_Duration = std::chrono::duration<float>(static_cast<float>(frameCount) / sampleRate);
	}

	void AudioFile::Play() {

		ma_result result;
		result = ma_sound_start(m_Sound.get());
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to initaialize {0}", (char*)result);
		}
	}

	void AudioFile::Pause() {

		ma_result result;
		result = ma_sound_stop(m_Sound.get());

		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to pause {0}", (char*)result);
		}
	}

	void AudioFile::Seek(float seconds) {

		ma_result result;
		result = ma_sound_seek_to_pcm_frame(m_Sound.get(), static_cast<uint64_t>(seconds * m_Engine->GetSampleRate()));
		HUB_ASSERT(result == MA_SUCCESS, "Sound failed to seek to pcm frame")
	}

	void AudioFile::SetLooping(bool isLooping) {
		ma_sound_set_looping(m_Sound.get(), isLooping);
	}

	float AudioFile::GetCursorPosition() {

		ma_result result;
		float CursorPos;
		result = ma_sound_get_cursor_in_seconds(m_Sound.get(), &CursorPos);
		HUB_ASSERT(result == MA_SUCCESS, "Sound failed to get cursor position");

		return CursorPos;
	}
}
#include "MusicPlayer.h"
#include "Core/Assert.h"

namespace taskhub {

	MusicPlayer::MusicPlayer() 
		: m_Engine(std::make_unique<ma_engine>()), m_Sound(std::make_unique<ma_sound>())
	{
		ma_result result;
		result = ma_engine_init(nullptr, m_Engine.get());
		if (result != MA_SUCCESS) {

			HUB_CORE_WARN("engine failed to initialize {0}", (char*)result);
		}
	}

	MusicPlayer::~MusicPlayer() {
		ma_engine_uninit(m_Engine.get());
		ma_sound_uninit(m_Sound.get());
	}

	void MusicPlayer::Load(const std::string& filepath) {

		ma_result result;
		
		result = ma_sound_init_from_file(m_Engine.get(), filepath.c_str(), 0, nullptr, nullptr, m_Sound.get());
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to initaialize {0}", (char*)result);
		}
	}

	void MusicPlayer::Play() {
		ma_result result;
		result = ma_sound_start(m_Sound.get());
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to initaialize {0}", (char*)result);
		}
	}

	void MusicPlayer::Pause() {

		ma_result result;
		result = ma_sound_stop(m_Sound.get());

		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to stop {0}", (char*)result);
		}
	}

	void MusicPlayer::SetVolume(float volume) {

		ma_result result;
		result = ma_engine_set_volume(m_Engine.get(), volume);

		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("engine failed to set volume {0}", (char*)result);
		}
	}

	void MusicPlayer::Seek(float seconds) {
		
		ma_uint64 frameIndex = static_cast<ma_uint64>(seconds * m_Engine->sampleRate);
		
		ma_result result;
		result = ma_sound_seek_to_pcm_frame(m_Sound.get(), frameIndex);
		
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to seek to pcm frame {0}", (char*)result);
		}
	}

	// Returns length in seconds
	float MusicPlayer::GetSongLength() {
		
		ma_uint64 lengthInFrames;
		ma_result result;
		result = ma_sound_get_length_in_pcm_frames(m_Sound.get(), &lengthInFrames);
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to seek to pcm frame {0}", (char*)result);
		}

		ma_uint32 sampleRate = ma_engine_get_sample_rate(m_Engine.get());
		float lengthInSeconds = static_cast<float>(lengthInFrames) / sampleRate;

		return lengthInSeconds;
	}

	// Returns Position in the song
	float MusicPlayer::GetCursorPosition() {

		ma_uint64 cursor;
		ma_result result;
		result = ma_sound_get_cursor_in_pcm_frames(m_Sound.get(), &cursor);
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("sound failed to get cursor in pcm frames {0}", (void*)result);
		}

		ma_uint32 sampleRate = ma_engine_get_sample_rate(m_Engine.get());
		float positionInSeconds = static_cast<float>(cursor) / sampleRate;

		return positionInSeconds;
	}
}
#pragma once

#include "AudioPlayer.h"
#include "miniaudio.h"
#include <filesystem>

namespace taskhub {

	class MusicPlayer : public AudioPlayer {
	public:
		MusicPlayer();
		virtual ~MusicPlayer();

		void Load(const std::string& filepath) override;

		void Play() override;
		void Pause() override;

		void SetVolume(float volume) override;
		
		void Seek(float seconds);

		float GetSongLength();
		float GetCursorPosition();

	private:
		std::unique_ptr<ma_engine> m_Engine;
		std::unique_ptr<ma_sound> m_Sound;
	};
}
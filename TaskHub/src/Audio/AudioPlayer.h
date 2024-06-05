#pragma once

#include <string>

namespace taskhub {

	class AudioPlayer {
	public:
		virtual ~AudioPlayer() = default;

		virtual void Load(const std::string& filepath) = 0;

		virtual void Play() = 0;
		virtual void Pause() = 0;

		virtual void SetVolume(float volume) = 0;
	};
}

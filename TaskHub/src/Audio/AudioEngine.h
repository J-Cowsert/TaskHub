#pragma once
#include <memory>
#include <string>

namespace taskhub {

	class AudioEngine {
	public:
		virtual ~AudioEngine() {}

		virtual void SetGlobalVolume(float volume) = 0;
		virtual uint32_t GetSampleRate() = 0;
		virtual void* GetContext() = 0;
	};
}
#pragma once
#include <memory>
#include <string>

namespace taskhub {

	class AudioEngine {
	public:
		virtual ~AudioEngine() = default;

		virtual void SetGlobalVolume(float volume) = 0;
		virtual uint32_t GetSampleRate() const = 0;
		virtual void* GetContext() const = 0;
	};
}
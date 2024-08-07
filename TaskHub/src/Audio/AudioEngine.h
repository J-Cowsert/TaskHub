#pragma once
#include "miniaudio.h"
#include <memory>

namespace taskhub {

	class AudioEngine {
	public:
		~AudioEngine();

		AudioEngine(AudioEngine& other) = delete;
		void operator=(const AudioEngine&) = delete;

		void SetGlobalVolume(float volume);
		uint32_t GetSampleRate() const;

		static AudioEngine* GetInstance();
		ma_engine* GetEngineHandle() const { return m_EngineHandle.get(); }
		
	private:
		AudioEngine();

	private:
		static AudioEngine* s_Instance;

		std::unique_ptr<ma_engine> m_EngineHandle;	
	};
}
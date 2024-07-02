#pragma once
#include "HubAudioEngine.h"
#include "miniaudio.h"
#include <memory>

namespace taskhub {

	class HubAudioEngine {
	public:
		~HubAudioEngine();

		HubAudioEngine(HubAudioEngine& other) = delete;
		void operator=(const HubAudioEngine&) = delete;

		void SetGlobalVolume(float volume);
		uint32_t GetSampleRate() const;

		static HubAudioEngine* GetInstance();
		ma_engine* GetEngineHandle() const { return m_EngineHandle.get(); }

	private:
		HubAudioEngine();

	private:
		static HubAudioEngine* s_Instance;

		std::unique_ptr<ma_engine> m_EngineHandle;	
	};
}
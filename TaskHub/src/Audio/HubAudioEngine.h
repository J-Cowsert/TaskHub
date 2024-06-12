#pragma once
#include "AudioEngine.h"
#include "miniaudio.h"
#include <memory>

namespace taskhub {

	class HubAudioEngine : public AudioEngine {
		friend class AudioFile;
		
	public:
		HubAudioEngine();
		virtual ~HubAudioEngine() override;

		void SetGlobalVolume(float volume) override;
		uint32_t GetSampleRate() override;

	private:
		inline void* GetContext() override { return m_Engine.get(); }

	private:
		std::shared_ptr<ma_engine> m_Engine;	
	};
}
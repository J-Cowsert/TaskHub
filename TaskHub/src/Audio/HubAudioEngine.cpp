#include "HubAudioEngine.h"
#include "Core/Assert.h"

namespace taskhub {

	HubAudioEngine::HubAudioEngine() 
		: m_Engine(std::make_shared<ma_engine>())
	{
		ma_result result = ma_engine_init(nullptr, m_Engine.get());
		if (result != MA_SUCCESS)
			HUB_CORE_WARN("Engine failed to initialize: {0}", (int)result);
	}

	HubAudioEngine::~HubAudioEngine() {
		// Consider if the engine might deconstruct while an audio file is still using it.
		ma_engine_uninit(m_Engine.get());
	}

	void HubAudioEngine::SetGlobalVolume(float volume) {

		ma_result result;
		result = ma_engine_set_volume(m_Engine.get(), volume);
		
		if (result != MA_SUCCESS) {
			HUB_CORE_WARN("engine failed to set volume {0}", (char*)result);
		}
	}

	uint32_t HubAudioEngine::GetSampleRate() {

		uint32_t sampleRate = ma_engine_get_sample_rate(m_Engine.get());
		return sampleRate;
	}
}
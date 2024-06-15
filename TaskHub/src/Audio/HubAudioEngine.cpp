#include "HubAudioEngine.h"
#include "Core/Assert.h"

namespace taskhub {

	HubAudioEngine::HubAudioEngine() 
		: m_Engine(std::make_shared<ma_engine>())
	{
		ma_result result = ma_engine_init(nullptr, m_Engine.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Engine failed to initialize");
	}

	HubAudioEngine::~HubAudioEngine() {
		ma_engine_uninit(m_Engine.get());
	}

	void HubAudioEngine::SetGlobalVolume(float volume) {

		ma_result result;
		result = ma_engine_set_volume(m_Engine.get(), volume);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Engine failed to set volume");
	}

	uint32_t HubAudioEngine::GetSampleRate() const {
		return ma_engine_get_sample_rate(m_Engine.get());
	}
}
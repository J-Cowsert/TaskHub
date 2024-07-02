#include "HubAudioEngine.h"
#include "Core/Assert.h"

namespace taskhub {

	HubAudioEngine* HubAudioEngine::s_Instance = nullptr;

	HubAudioEngine::HubAudioEngine()
		: m_EngineHandle(std::make_unique<ma_engine>())
	{
		ma_result result = ma_engine_init(nullptr, m_EngineHandle.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Engine failed to initialize");
	}

	HubAudioEngine::~HubAudioEngine() {
		ma_engine_uninit(m_EngineHandle.get());
	}

	HubAudioEngine* HubAudioEngine::GetInstance() {
		
		if (s_Instance == nullptr) {
			s_Instance = new HubAudioEngine();
		}
		return s_Instance;
	}

	void HubAudioEngine::SetGlobalVolume(float volume) {
		
		ma_result result;
		result = ma_engine_set_volume(m_EngineHandle.get(), volume);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Engine failed to set volume");
	}

	uint32_t HubAudioEngine::GetSampleRate() const {
		return ma_engine_get_sample_rate(m_EngineHandle.get());
	}
}
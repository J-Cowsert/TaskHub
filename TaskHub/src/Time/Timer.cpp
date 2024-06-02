#include "Timer.h"

namespace taskhub {

	Timer::Timer()
		: m_Running(false), m_Duration(0.0f), m_Elapsed(0.0f)
	{
	}

	void Timer::SetTimer(float duration) {
		m_Duration = std::chrono::duration<float>(duration);
	}

	void Timer::Start() {
		
		if (!m_Running) {
			m_Start = std::chrono::high_resolution_clock::now();
			m_Running = true;
		}
	}

	void Timer::Stop() {

		if (m_Running) {
			auto now = std::chrono::high_resolution_clock::now();
			m_Elapsed += now - m_Start;
			m_Running = false;
		}
	}

	void Timer::Reset() {
		m_Running = false;
		m_Elapsed = std::chrono::duration<float>(0.0f);
	}

	float Timer::GetRemainingTime() {

		if (!m_Running) {
			return std::chrono::duration<float>(m_Duration - m_Elapsed).count();
		}
		else {
			auto now = std::chrono::high_resolution_clock::now();
			float duration = std::chrono::duration<float>(m_Duration - m_Elapsed - (now - m_Start)).count();
			float remTime = (duration >= 0) ? duration : 0.0f;
			return remTime;
		}
	}

	float Timer::GetElapsedTime() {

		if (!m_Running) {
			return m_Elapsed.count();
		}
		else {
			auto now = std::chrono::high_resolution_clock::now();
			return (m_Elapsed + (now - m_Start)).count();
		}
	}
}
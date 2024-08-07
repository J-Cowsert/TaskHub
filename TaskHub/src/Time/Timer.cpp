#include "Timer.h"

namespace taskhub {

	Timer::Timer(std::function<void()> timerCompletedCallback)
		: m_Running(false), m_Completed(false), m_Duration(std::chrono::duration<float>(0.0f)),
		  m_Elapsed(std::chrono::duration<float>(0.0f)), m_TimerCompletedCallback(timerCompletedCallback)
	{
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
		m_Completed = false;
		m_Elapsed = std::chrono::duration<float>(0.0f);
	}

	// Must be called consecutivly for timer to function properly. 
	// TODO: Redesign class to be multithreaded to function on its own
	float Timer::GetRemainingTime() {

		if (!m_Running) {
			return std::chrono::duration<float>(m_Duration - m_Elapsed).count();
		}
		else {
			auto now = std::chrono::high_resolution_clock::now();
			float remainingTime = std::chrono::duration<float>(m_Duration - m_Elapsed - (now - m_Start)).count();

			// 
			if (remainingTime <= 0) {
				m_Completed = true;
				if (m_TimerCompletedCallback)
					m_TimerCompletedCallback();
			}

			return (remainingTime >= 0) ? remainingTime : 0.0f;
		}
	}

	float Timer::GetElapsedTime() const {

		if (!m_Running) {
			return m_Elapsed.count();
		}
		else {
			auto now = std::chrono::high_resolution_clock::now();
			return (m_Elapsed + (now - m_Start)).count();
		}
	}
}
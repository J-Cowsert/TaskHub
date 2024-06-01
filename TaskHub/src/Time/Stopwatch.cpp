#include "Stopwatch.h"

namespace taskhub {

	Stopwatch::Stopwatch()
		: m_Running(false), m_ElapsedTime(0.0f) 
	{
	}

	void Stopwatch::Start() {

		if (!m_Running) {
			m_Start = std::chrono::high_resolution_clock::now();
			m_Running = true;
		}
	}

	void Stopwatch::Stop() {

		if (m_Running) {
			auto end = std::chrono::high_resolution_clock::now();
			m_ElapsedTime += std::chrono::duration<float>(end - m_Start).count();
			m_Running = false;
		}
	}

	void Stopwatch::Reset() {
		m_Running = false;
		m_ElapsedTime = 0.0f;
	}

	float Stopwatch::GetElapsedTime() {

		if (m_Running) {
			auto now = std::chrono::high_resolution_clock::now();
			return m_ElapsedTime + std::chrono::duration<float>(now - m_Start).count();
		}
		return m_ElapsedTime;
	}
}
#pragma once

#include <chrono>

namespace taskhub {

	class Timer {
	public:
		Timer();
		~Timer() = default;

		void SetTimer(float duration); // Only In seconds for now 

		void Start();
		void Stop();
		void Reset();

		float GetRemainingTime();
		float GetElapsedTime();

	private:
		bool m_Running;
		std::chrono::duration<float> m_Duration;
		std::chrono::duration<float> m_Elapsed;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
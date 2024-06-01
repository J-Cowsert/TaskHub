#pragma once

#include <chrono>

namespace taskhub {

	class Stopwatch {
	public:
		Stopwatch();
		~Stopwatch() = default;

		void Start();
		void Stop();
		void Reset();

		float GetElapsedTime();

	private:
		bool m_Running;
		float m_ElapsedTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
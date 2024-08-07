#pragma once

#include <chrono>
#include <functional>

namespace taskhub {

	class Timer {
	public:
		explicit Timer(std::function<void()> timerCompletedCallback);
		~Timer() = default;

		template<typename DurationType>
		void SetTimer(float duration) {
			auto durationInInt = static_cast<typename DurationType::rep>(duration);
			m_Duration = DurationType(durationInInt);
		}

		void SetCompletedCallback(std::function<void()>& callback) { m_TimerCompletedCallback = callback; }
		
		void Start();
		void Stop();
		void Reset();

		float GetRemainingTime();
		float GetElapsedTime() const;

		bool IsCompleted() const { return m_Completed; }
		bool IsRunning() const { return m_Running; }

	private:
		bool m_Running;
		bool m_Completed;

		std::chrono::duration<float> m_Duration;
		std::chrono::duration<float> m_Elapsed;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;

		std::function<void()> m_TimerCompletedCallback;
	};
}
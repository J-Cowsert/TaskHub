#pragma once

#include <chrono>
#include <string>

namespace taskhub {

	class Timer {
	public:

		Timer() {
			Reset();
		}

		void Reset() {
			m_Start = std::chrono::steady_clock::now();
		}

		float Elapsed() {
			return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_Start).count();
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
	};

	class Clock {
	public:

		Clock(std::string timezone)
			:m_TimeZone(timezone)
		{
		}

		std::chrono::hh_mm_ss<std::chrono::seconds> GetTime() {

			std::chrono::zoned_time zone{ m_TimeZone, std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()) };
			auto localTime = zone.get_local_time();
			auto timeOfDay = localTime - std::chrono::floor<std::chrono::days>(localTime);

			std::chrono::hh_mm_ss time_of_day{ timeOfDay };
			return time_of_day;
		}

	private:
		std::string m_TimeZone;
	};
}
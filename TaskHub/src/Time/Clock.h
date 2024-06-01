#pragma once

#include <chrono>
#include <string>

namespace taskhub {

	class Clock {
	public:
		Clock(const std::string timezone)
			: m_TimeZone(timezone)
		{
		}

		~Clock() = default;

		std::string GetTheTime();
		std::string GetTheDate();

	private:
		std::string FormatTime(const std::chrono::hh_mm_ss<std::chrono::seconds>& time);

	private:
		std::string m_TimeZone;
	};
}
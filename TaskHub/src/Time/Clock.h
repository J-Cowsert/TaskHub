#pragma once

#include <chrono>
#include <string>

namespace taskhub {


	class Clock {
	public:
		Clock(std::string timezone)
			: m_TimeZone(timezone)
		{
		}

		std::string GetCurrentTime();
		std::string GetCurrentDate();

	private:
		std::string FormatTime(const std::chrono::hh_mm_ss<std::chrono::seconds>& time);

	private:
		std::string m_TimeZone;
	};
}
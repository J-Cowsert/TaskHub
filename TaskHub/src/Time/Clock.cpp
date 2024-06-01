#include "Clock.h"
#include <sstream>

namespace taskhub {

	std::string Clock::GetTheTime() {

		std::chrono::zoned_time zone{ m_TimeZone, std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()) };
		auto time = zone.get_local_time();
		auto timeOfDay = time - std::chrono::floor<std::chrono::days>(time);

		std::chrono::hh_mm_ss time_of_day{ timeOfDay };

		return FormatTime(time_of_day);
	}

	std::string Clock::GetTheDate() { // TODO: use chrono calender stuff instead

		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		std::tm* local_time = std::localtime(&now_time);					// not thread safe

		std::ostringstream oss;
		oss << (local_time->tm_year + 1900) << '-'
			<< (local_time->tm_mon + 1) << '-'
			<< local_time->tm_mday;

		return oss.str();
	}

	std::string Clock::FormatTime(const std::chrono::hh_mm_ss<std::chrono::seconds>& time) {

		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << time.hours().count() << ":"
			<< std::setw(2) << std::setfill('0') << time.minutes().count() << ":"
			<< std::setw(2) << std::setfill('0') << time.seconds().count();
		return oss.str();
	}
}
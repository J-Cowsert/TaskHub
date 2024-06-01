#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace taskhub {

	class Log {
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define HUB_CORE_CRITICAL(...) ::taskhub::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define HUB_CORE_ERROR(...)    ::taskhub::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HUB_CORE_WARN(...)     ::taskhub::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HUB_CORE_INFO(...)     ::taskhub::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HUB_CORE_TRACE(...)    ::taskhub::Log::GetCoreLogger()->trace(__VA_ARGS__)
		
#define HUB_CRITICAL(...)	   ::taskhub::Log::GetClientLogger()->critical(__VA_ARGS__)
#define HUB_ERROR(...)	       ::taskhub::Log::GetClientLogger()->error(__VA_ARGS__)
#define HUB_WARN(...)	       ::taskhub::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HUB_INFO(...)	       ::taskhub::Log::GetClientLogger()->info(__VA_ARGS__)
#define HUB_TRACE(...)	       ::taskhub::Log::GetClientLogger()->trace(__VA_ARGS__)
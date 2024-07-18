#pragma once

#include "Log.h"

#ifdef HUB_PLATFORM_WINDOWS
#else
#error taskhub currently only supports Windows
#endif // HUB_PLATFORM_WINDOWS

#ifdef HUB_DEBUG
#define HUB_ENABLE_ASSERTS
#endif // HUB_DEBUG

#ifdef HUB_ENABLE_ASSERTS
	#define HUB_ASSERT(x, ...) { if(!(x)) { HUB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HUB_CORE_ASSERT(x, ...) { if(!(x)) { HUB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HUB_ASSERT(x, ...)
	#define HUB_CORE_ASSERT(x, ...)
#endif // HUB_ENABLE_ASSERTS
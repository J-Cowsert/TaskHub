#pragma once

#include "Base.h"
#include "Log.h"

#ifdef HUB_ENABLE_ASSERTS
	#define HUB_ASSERT(x, ...) { if(!(x)) { HUB_ERROR("Assertion Failed: {0}", __VA_ARGS__); HUB_DEBUGBREAK(); } }
	#define HUB_CORE_ASSERT(x, ...) { if(!(x)) { HUB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HUB_DEBUGBREAK(); } }
#else
	#define HUB_ASSERT(x, ...)
	#define HUB_CORE_ASSERT(x, ...)
#endif // HUB_ENABLE_ASSERTS
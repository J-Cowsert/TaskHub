#pragma once

#ifdef HUB_DEBUG
	#if defined(HUB_PLATFORM_WINDOWS)
		#define HUB_DEBUGBREAK() __debugbreak()
	#else
		#error taskhub currently only supports Windows
	#endif
	#define HUB_ENABLE_ASSERTS
#else
	#define HUB_DEBUGBREAK()
#endif

#include "Log.h"
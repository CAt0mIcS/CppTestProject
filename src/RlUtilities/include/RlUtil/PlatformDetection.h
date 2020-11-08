#ifndef UT_PLATFORMDETECTION_H
#define UT_PLATFORMDETECTION_H

#if defined(_WIN32) || defined(WIN32)
	#define RL_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#define RL_PLATFORM_IPHONESIMULATOR
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define RL_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define RL_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define RL_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define RL_PLATFORM_LINUX
#else
	#error "Unknown platform!"
#endif


#endif

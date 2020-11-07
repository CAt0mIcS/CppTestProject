#ifndef RLR_TYPEDEFINES_H
#define RLR_TYPEDEFINES_H

#if defined(_WIN32) || defined(WIN32)
	#include <DirectXMath.h>
	namespace At0::Reyal
	{
		using WindowHandle = HWND;
		using Matrix = DirectX::XMMATRIX;
	}
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	
	#if TARGET_IPHONE_SIMULATOR
		#error "IPhone Simulator is currently not supported"
	#elif TARGET_OS_IPHONE
		#error "IPhone is currently not supported"
	#elif TARGET_OS_MAC
		#error "MacOS is currently not supported"
	#endif
#elif defined(__linux__)
	#error "Linux is currently not supported"
#elif defined(__unix__)
	#error "Unix is currently not supported"
#elif defined(__POSIX__)
	#error "POSIX is currently not supported"
#else
	#error "Platform Detection was unable to recognize your operating system"
#endif

#endif // RLR_TYPEDEFINES_H

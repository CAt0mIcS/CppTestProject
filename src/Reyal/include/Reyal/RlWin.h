#ifndef RL_RLWIN_H
#define RL_RLWIN_H



#ifdef _WIN32

	#include <sdkddkver.h>
	#define NOATOM
	#define NOGDICAPMASKS
	#define NOMETAFILE
	#define NOMINMAX
	#define NOOPENFILE
	#define NORASTEROPS
	#define NOSCROLL
	#define NOSOUND
	#define NOSYSMETRICS
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOCRYPT
	#define NOMCX

	#include <Windows.h>

	#undef NOATOM
	#undef NOGDICAPMASKS
	#undef NOMETAFILE
	#undef NOMINMAX
	#undef NOOPENFILE
	#undef NORASTEROPS
	#undef NOSCROLL
	#undef NOSOUND
	#undef NOSYSMETRICS
	#undef NOWH
	#undef NOCOMM
	#undef NOKANJI
	#undef NOCRYPT
	#undef NOMCX
#else
	#error "Only Windows is currently supported!"
#endif


#endif // RL_RLWIN_H

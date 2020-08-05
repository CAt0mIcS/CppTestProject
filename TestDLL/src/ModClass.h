#pragma once

#ifdef TD_BULD
	#define TD_API extern "C" __declspec(dllexport)
#else
	#define TD_API extern "C" __declspec(dllimport)
#endif

#include "../../TPTools/src/IGun.h"


//struct TD_API ModClass
//{
TD_API void OnGameInit();
//};


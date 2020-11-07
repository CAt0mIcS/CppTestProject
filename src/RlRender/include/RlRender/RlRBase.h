#ifndef RLR_RLRBASE_H
#define RLR_RLRBASE_H

#include <sal.h>

#ifdef RLR_BUILD
	#define RLR_API __declspec(dllexport)
#else
	#define RLR_API __declspec(dllimport)
#endif

#endif // RLR_RLRBASE_H
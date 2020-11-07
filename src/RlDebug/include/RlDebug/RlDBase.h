#ifndef RLD_RLDBASE_H
#define RLD_RLDBASE_H

#ifdef RLD_BUILD
	#define RLD_API __declspec(dllexport)
#else
	#define RLD_API __declspec(dllimport)
#endif

#endif // RLD_RLDBASE_H
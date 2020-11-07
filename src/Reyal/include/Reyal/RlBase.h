#ifndef RL_RLBASE_H
#define RL_RLBASE_H


#ifdef RL_BUILD
	#define RL_API __declspec(dllexport)
#else
	#define RL_API __declspec(dllimport)
#endif

#endif // RL_RLBASE_H
#ifndef UT_UTBASE_H
#define UT_UTBASE_H

#define RLU_BUILD_AS_DLL 0

#if RLU_BUILD_AS_DLL
	#ifdef RLU_BUILD
		#if defined(_MSC_VER)
			#define RLU_API __declspec(dllexport)
		#elif defined(__GNUC__) || defined(__clang__)
			#define RLU_API __attribute__((dllexport))
		#else
			#define RLU_API
		#endif
	#else
		#if defined(_MSC_VER)
			#define RLU_API __declspec(dllimport)
		#elif defined(__GNUC__) || defined(__clang__)
			#define RLU_API __attribute__((dllimport))
		#else
			#define RLU_API
		#endif
	#endif
#else
	#define RLU_API
#endif

#endif // UT_UTBASE_H
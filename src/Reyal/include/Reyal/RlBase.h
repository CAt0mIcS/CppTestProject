#ifndef RL_RLBASE_H
#define RL_RLBASE_H

#include <memory>
#include <assert.h>


#ifdef RL_BUILD
	#if defined(_MSC_VER)
		#define RL_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RL_API __attribute__((dllexport))
	#else
		#define RL_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RL_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RL_API __attribute__((dllimport))
	#else
		#define RL_API
	#endif
#endif


namespace At0::Reyal
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Scope<T> MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif // RL_RLBASE_H

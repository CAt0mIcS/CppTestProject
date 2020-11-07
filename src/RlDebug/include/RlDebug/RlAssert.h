#ifndef RLD_RLASSERT_H
#define RLD_RLASSERT_H

#ifdef ZL_DEBUG

#include "ReyalLogger.h"

#include <assert.h>
#include "RlDBase.h"


namespace At0::Reyal::Debug
{
	class RLD_API RlAssert
	{
	public:
		template<typename... Args>
		static std::wstring Assert(const std::string& str, Args&&... args)
		{
			std::wstring assertStr = Util::MultiByteToWideChar(Util::SerializeString(str, std::forward<Args>(args)...));
			RL_LOG_CRITICAL("Assertion Failed with Message: {0}", assertStr);
			return assertStr;
		}
	};
}


#define RL_ASSERT(condition, msg, ...)	if(!(condition)) _wassert(::At0::Reyal::Debug::RlAssert::Assert(msg, __VA_ARGS__).c_str(), _CRT_WIDE(__FILE__), (unsigned int)__LINE__)
#define RL_MEXPECTS(expected, msg)		RL_ASSERT(expected, "[Expected] {0}\n{1}", #expected, msg)
#define RL_EXPECTS(expected)			RL_ASSERT(expected, "[Expected] {0}", #expected)
#else
#define RL_ASSERT(condition, msg, ...)
#define RL_MEXPECTS(expected, msg)
#define RL_EXPECTS(expected)
#endif

#endif // RLD_RLASSERT_H
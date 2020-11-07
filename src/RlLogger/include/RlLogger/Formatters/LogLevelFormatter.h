#ifndef ZZL_LOGLEVELFORMATTER_H
#define ZZL_LOGLEVELFORMATTER_H

#include "Formatter.h"


namespace At0::Log
{
	class LogLevelFormatter : public Formatter
	{
	public:
		LogLevelFormatter() = default;

		virtual void Format(std::string& str, LogLevel logLvl) override
		{
#ifndef ZL_NO_LOG_LEVEL_OUT

			str.insert(1, s_LogLevelStr[(int)logLvl]);

#endif // !ZL_NO_LOG_LEVEL_OUT
		}

	private:
#ifndef ZL_NO_LOG_LEVEL_OUT

		inline static const std::string s_LogLevelStr[] = 
		{
			"TRC", "DBG", "LOG", "WRN", "ERR", "CRT", "UDF"
		};

#endif // !ZL_NO_LOG_LEVEL_OUT


	};
}

#endif // ZZL_LOGLEVELFORMATTER_H

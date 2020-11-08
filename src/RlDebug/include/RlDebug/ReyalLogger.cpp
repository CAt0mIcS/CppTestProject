#include "rldpch.h"
#include "RlDebug/ReyalLogger.h"

#include <RlLogger/Log.h>

#include "RlDebug/RlAssert.h"


namespace At0::Reyal::Debug
{

#if RL_ENABLE_LOGGING

	Log::FileLogger Logger::s_FileLogger;

	void Logger::Init(const std::string_view filepath, Log::LogLevel logLvl)
	{
		s_FileLogger = Log::FileLogger(filepath);
		s_FileLogger.SetLogLevel(logLvl);
		s_FileLogger.Open();

		// TODO: Throw exception here
		RL_EXPECTS(s_FileLogger.IsOpen());
	}

	void Logger::End()
	{
		s_FileLogger.Close();
	}

#endif
}
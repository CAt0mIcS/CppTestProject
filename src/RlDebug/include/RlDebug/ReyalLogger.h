#ifndef RLD_REYALLOGGER_H
#define RLD_REYALLOGGER_H


#include <RlUtil/CoreConfig.h>


#if RL_ENABLE_LOGGING

#include <RlLogger/Log.h>
#include "RlDBase.h"
#include <string>

#pragma warning(disable : 4251)


namespace At0::Reyal::Debug
{
	class RLD_API Logger
	{
	public:
		/// <summary>
		/// Getter for the file logger
		/// </summary>
		/// <returns>The file logger</returns>
		static Log::FileLogger& GetFileLogger() { return s_FileLogger; }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		static void Init(const std::string_view filepath, Log::LogLevel logLvl);

		/// <summary>
		/// Closes the filestream and deletes the loggers
		/// </summary>
		static void End();

	private:
		static Log::FileLogger s_FileLogger;
	};

}

#endif

/// <summary>
/// Define logging macros here
/// </summary>
#if RL_ENABLE_LOGGING

	#define RL_LOG_BEGIN(path, loglvl)	::At0::Reyal::Debug::Logger::Init(path, loglvl)

	#define RL_LOG_TRACE(msg, ...)		::At0::Reyal::Debug::Logger::GetFileLogger().Trace(msg, __VA_ARGS__)
	#define RL_LOG_DEBUG(msg, ...)		::At0::Reyal::Debug::Logger::GetFileLogger().Debug(msg, __VA_ARGS__)
	#define RL_LOG_INFO(msg, ...)		::At0::Reyal::Debug::Logger::GetFileLogger().Info(msg, __VA_ARGS__)
	#define RL_LOG_WARN(msg, ...)		::At0::Reyal::Debug::Logger::GetFileLogger().Warn(msg, __VA_ARGS__)
	#define RL_LOG_ERROR(msg, ...)		::At0::Reyal::Debug::Logger::GetFileLogger().Error(msg, __VA_ARGS__)
	#define RL_LOG_CRITICAL(msg, ...)	::At0::Reyal::Debug::Logger::GetFileLogger().Critical(msg, __VA_ARGS__)

	#define RL_FL_LOGGER_OPEN()			::At0::Reyal::Debug::Logger::GetFileLogger().IsOpen()

	#define RL_LOG_END()				::At0::Reyal::Debug::Logger::End();
#else
	#define RL_LOG_BEGIN(path, loglevel)
	
	#define RL_LOG_TRACE(msg, ...)	
	#define RL_LOG_DEBUG(msg, ...)	
	#define RL_LOG_INFO(msg, ...)		
	#define RL_LOG_WARN(msg, ...)		
	#define RL_LOG_ERROR(msg, ...)	
	#define RL_LOG_CRITICAL(msg, ...)

	#define RL_FL_LOGGER_OPEN()

	#define RL_LOG_END()
#endif

#endif // RLD_REYALLOGGER_H

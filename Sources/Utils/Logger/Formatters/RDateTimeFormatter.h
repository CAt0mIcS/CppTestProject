#pragma once

#include "RFormatter.h"
#include <chrono>
#include <mutex>
#include <sstream>

namespace At0::VulkanTesting
{
	class DateTimeFormatter : public Formatter
	{
	public:
		/// <summary>
		/// Default DateTimeFormatter Constructor
		/// </summary>
		DateTimeFormatter() = default;

		/// <summary>
		/// Adds the date and time to the string
		/// </summary>
		/// <param name="str">Is the string which will be modified</param>
		/// <param name="logLvl">Is the current log level, unused in this function specification</param>
		virtual void Format(std::string& str, LogLevel logLvl) override
		{
#ifndef ZL_NO_DATETIME_OUT

	#ifdef ZL_NO_LOG_LEVEL_OUT
			static constexpr unsigned int s_InsertIdx = 1;
	#else
			static constexpr unsigned int s_InsertIdx = 5;
			str.insert(s_InsertIdx - 1, " ");
	#endif


			time_t rawTime;
			struct tm timeInfo;
			char buff[80];

			time(&rawTime);
			timeInfo = *localtime(&rawTime);

			strftime(buff, sizeof(buff), "%d-%m-%Y %H:%M:%S", &timeInfo);

			long long ms;
			{
				std::scoped_lock lock(s_ChronoMutex);
				ms = (std::chrono::duration_cast<std::chrono::milliseconds>(
						  std::chrono::high_resolution_clock::now().time_since_epoch()) %
					  1000)
						 .count();
			}
			std::ostringstream oss;
			if (ms < 10)
			{
				oss << buff << ":00" << ms;
			}
			else if (ms < 100)
			{
				oss << buff << ":0" << ms;
			}
			else
			{
				oss << buff << ':' << ms;
			}

			str.insert(s_InsertIdx, oss.str());
#endif
		}

	private:
#if !defined(ZL_NO_DATE_OUT) && !defined(ZL_NO_TIME_OUT) || !defined(ZL_LOG_NON_THREAD_SAVE)

		std::mutex s_ChronoMutex;

#endif	// !ZL_LOG_NON_THREAD_SAVE || !ZL_NO_DATE_OUT
	};

}  // namespace At0::VulkanTesting

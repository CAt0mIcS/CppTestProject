﻿#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <sstream>

#include "../Formatters/RBracketFormatter.h"
#include "../Formatters/RLogLevelFormatter.h"
#include "../Formatters/RDateTimeFormatter.h"
#include "../Formatters/RNullTerminatorFormatter.h"

#include "../RLogLevel.h"


#pragma warning(disable : 4251)

namespace At0::VulkanTesting
{
	using LogMessageType = LogLevel;

	class BaseLogger
	{
	public:
		/// <summary>
		/// Sets the log severity level of this logger
		/// </summary>
		/// <param name="level">Is the new log level</param>
		void SetLogLevel(LogLevel level) { m_LogLevel = level; }

		/// <summary>
		/// Getter for the current log level
		/// </summary>
		/// <returns>The current log level of this logger</returns>
		LogLevel GetLogLevel() const { return m_LogLevel; }

		/// <summary>
		/// Opens the stream to write to
		/// </summary>
		virtual void Open(const char* filepath) {};

		/// <summary>
		/// Flushes the output stream
		/// </summary>
		virtual void Flush() = 0;

		/// <summary>
		/// Closes the output stream
		/// </summary>
		virtual void Close() {};

		/// <summary>
		/// Logs a trace message, will only be logged if the loglevel is trace
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Trace(std::string_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Trace))
				return;

			Log(FormatMessage(str, LogMessageType::Trace, std::forward<Args>(args)...),
				LogMessageType::Trace);
		}

		/// <summary>
		/// Logs a debug message, will only be logged if the loglevel >= Debug
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Debug(std::string_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(str, LogMessageType::Debug, std::forward<Args>(args)...),
				LogMessageType::Debug);
		}

		/// <summary>
		/// Logs a info message, will only be logged if the loglevel is >= Information
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Info(std::string_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Information))
				return;

			Log(FormatMessage(str, LogMessageType::Information, std::forward<Args>(args)...),
				LogMessageType::Information);
		}

		/// <summary>
		/// Logs a warning message, will only be logged if the loglevel >= Warning
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Warn(std::string_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Warning))
				return;

			Log(FormatMessage(str, LogMessageType::Warning, std::forward<Args>(args)...),
				LogMessageType::Warning);
		}

		/// <summary>
		/// Logs a error message, will only be logged if the loglevel is >= "Error"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Error(std::string_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Error))
				return;

			Log(FormatMessage(str, LogMessageType::Error, std::forward<Args>(args)...),
				LogMessageType::Error);
		}

		/// <summary>
		/// Logs a critical message, will always be logged if the loglevel is not "None"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Critical(std::string_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Critical))
				return;

			Log(FormatMessage(str, LogMessageType::Critical, std::forward<Args>(args)...),
				LogMessageType::Critical);
		}


		/// <summary>
		/// Logs a trace message, will only be logged if the loglevel is trace
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Trace(const std::wstring_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Trace))
				return;

			Log(FormatMessage(
				String::ConvertUtf8(str), LogMessageType::Trace, std::forward<Args>(args)...),
				LogMessageType::Trace);
		}

		/// <summary>
		/// Logs a debug message, will only be logged if the loglevel >= Debug
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Debug(const std::wstring_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Debug))
				return;

			Log(FormatMessage(
				String::ConvertUtf8(str), LogMessageType::Debug, std::forward<Args>(args)...),
				LogMessageType::Debug);
		}

		/// <summary>
		/// Logs a info message, will only be logged if the loglevel is >= Information
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Info(const std::wstring_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Information))
				return;

			Log(FormatMessage(String::ConvertUtf8(str), LogMessageType::Information,
				std::forward<Args>(args)...),
				LogMessageType::Information);
		}

		/// <summary>
		/// Logs a warning message, will only be logged if the loglevel >= Warning
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Warn(const std::wstring_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Warning))
				return;

			Log(FormatMessage(
				String::ConvertUtf8(str), LogMessageType::Warning, std::forward<Args>(args)...),
				LogMessageType::Warning);
		}

		/// <summary>
		/// Logs a error message, will only be logged if the loglevel is >= "Error"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Error(const std::wstring_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Error))
				return;

			Log(FormatMessage(
				String::ConvertUtf8(str), LogMessageType::Error, std::forward<Args>(args)...),
				LogMessageType::Error);
		}

		/// <summary>
		/// Logs a critical message, will always be logged if the loglevel is not "None"
		/// </summary>
		/// <typeparam name="...Args">Is any type which has output operator for std::stringstream defined</typeparam>
		/// <param name="str">Is the initial string</param>
		/// <param name="...args">Are arguments to insert into the string</param>
		template<typename... Args>
		void Critical(const std::wstring_view str, Args&&... args)
		{
			if (!ShouldLog(LogMessageType::Critical))
				return;

			Log(FormatMessage(String::ConvertUtf8(str), LogMessageType::Critical,
				std::forward<Args>(args)...),
				LogMessageType::Critical);
		}

	protected:
		/// <summary>
		/// Writes message to output buffer
		/// </summary>
		/// <param name="message">Is the message to write</param>
		virtual void Log(std::string_view message, LogMessageType type) = 0;

		/// <summary>
		/// Base Logger Constructor
		/// </summary>
		BaseLogger() : m_LogLevel(LogLevel::None)
		{
			BracketFormatter* pBracketFormatter = new BracketFormatter();
			m_Formatters.push_back(pBracketFormatter);

			LogLevelFormatter* pLogLevelFormatter = new LogLevelFormatter();
			m_Formatters.push_back(pLogLevelFormatter);

			DateTimeFormatter* pDateFormatter = new DateTimeFormatter();
			m_Formatters.push_back(pDateFormatter);

			NullTerminatorFormatter* pNullTerminatorFormatter = new NullTerminatorFormatter();
			m_Formatters.push_back(pNullTerminatorFormatter);
		}

		/// <summary>
		/// Virtual BaseLogger Deconstructor
		/// </summary>
		virtual ~BaseLogger()
		{
			for (auto* formatter : m_Formatters)
			{
				if (formatter)
					delete formatter;
			}
		}

		/// <summary>
		/// Checks if a message with the msgType message type should be logged
		/// </summary>
		/// <param name="msgType">Is the log message type of the message</param>
		/// <returns>True if the message should be logged, false otherwise</returns>
		bool ShouldLog(LogMessageType msgType) { return msgType >= m_LogLevel; }

	private:
		/// <summary>
		/// Formats the message, inserts all arguments and calls the formatters
		/// </summary>
		/// <typeparam name="...Args">Are a list of any typed arguments</typeparam>
		/// <param name="str">Is the base string to insert the arguments into</param>
		/// <param name="...args">Are the arguments to insert</param>
		/// <returns>The formatted string ready for logging</returns>
		template<typename... Args>
		std::string FormatMessage(std::string_view str, LogMessageType msgLvl, Args&&... args)
		{
			std::string msg = SerializeString(str.data(), args...);

			for (auto formatter : m_Formatters)
			{
				formatter->Format(msg, msgLvl);
			}
			return msg;
		}

	protected:
		/// <summary>
		/// Is the loglevel by which it will be decided if the Logger should log the message
		/// </summary>
		LogLevel m_LogLevel;

	private:
		/// <summary>
		/// Is a list of formatters to call in order
		/// </summary>
		std::vector<Formatter*> m_Formatters;
	};
}  // namespace At0::VulkanTesting

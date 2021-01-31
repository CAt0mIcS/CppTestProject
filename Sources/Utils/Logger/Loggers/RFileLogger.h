#pragma once

#include <fstream>
#include "RBaseLogger.h"


namespace At0::VulkanTesting
{
	class FileLogger : public BaseLogger
	{
	public:
		/// <summary>
		/// Default FileLogger Constructor
		/// </summary>
		FileLogger() = default;

		/// <summary>
		/// File Logger Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the log file. Note: The file will be cleared when the logger starts</param>
		FileLogger(const char* filepath)
		{
			Open(filepath);
		}

		/// <summary>
		/// FileLogger Deconstructor, automatically closes the stream
		/// </summary>
		~FileLogger()
		{
			Close();
		}

		/// <summary>
		/// Checks if the file stream is open
		/// </summary>
		/// <returns>True if the stream to the log file is open, false otherwise</returns>
		bool IsOpen() const
		{
			return m_Writer.is_open();
		}

		/// <summary>
		/// Opens the FileStream
		/// </summary>
		virtual void Open(const char* filepath) override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			if (!m_Writer.is_open()) m_Writer.open(filepath);
		}

		/// <summary>
		/// Flushes the FileStream
		/// </summary>
		virtual void Flush() override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			if (m_Writer.is_open()) m_Writer.flush();
		}

		/// <summary>
		/// Closes the FileStream
		/// </summary>
		virtual void Close() override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			if (m_Writer.is_open()) m_Writer.close();
		}

	private:
		/// <summary>
		/// Writes the message to the output stream
		/// </summary>
		/// <param name="message">Is the message to output</param>
		virtual void Log(std::string_view message, LogMessageType type) override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			m_Writer << message << '\n';
		}

	private:
		std::ofstream m_Writer;

#ifndef ZL_LOG_NON_THREAD_SAVE

		std::mutex m_StreamMutex;

#endif
	};
}  // namespace At0::VulkanTesting

#ifndef ZZL_FILELOGGER_H
#define ZZL_FILELOGGER_H

#include <fstream>
#include "BaseLogger.h"


namespace At0::Log
{
	class FileLogger : public BaseLogger
	{
	public:
		/// <summary>
		/// Default FileLogger Constructor
		/// </summary>
		FileLogger()
			: m_FilePath("")
		{

		}

		/// <summary>
		/// Operator =
		/// </summary>
		/// <param name="other">Is the other file logger</param>
		/// <returns>The new file logger</returns>
		FileLogger& operator=(const FileLogger& other)
		{
			this->m_FilePath = other.m_FilePath;
			this->m_LogLevel = other.m_LogLevel;
			return *this;
		}

		/// <summary>
		/// File Logger Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the log file. Note: The file will be cleared when the logger starts</param>
		FileLogger(const std::string_view filepath)
			: m_FilePath(filepath)
		{

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
		/// Sets the file path to the log file. Note:
		/// This should only be used if the logger was created with the default Constructor to set the path once
		/// </summary>
		/// <param name="filepath"></param>
		void SetFilePath(const std::string_view filepath) { m_FilePath = filepath; }

		/// <summary>
		/// Opens the FileStream
		/// </summary>
		virtual void Open() override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			if(!m_Writer.is_open())
				m_Writer.open(m_FilePath);
		}

		/// <summary>
		/// Flushes the FileStream
		/// </summary>
		virtual void Flush() override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			if(m_Writer.is_open())
				m_Writer.flush();
		}

		/// <summary>
		/// Closes the FileStream
		/// </summary>
		virtual void Close() override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			if(m_Writer.is_open())
				m_Writer.close();
		}

	private:
		/// <summary>
		/// Writes the message to the output stream
		/// </summary>
		/// <param name="message">Is the message to output</param>
		virtual void Log(const std::string_view message) override
		{
#ifndef ZL_LOG_NON_THREAD_SAVE

			std::scoped_lock lock(m_StreamMutex);

#endif
			m_Writer << message << '\n';
		}

	private:
		std::ofstream m_Writer;
		std::string m_FilePath;

#ifndef ZL_LOG_NON_THREAD_SAVE

		std::mutex m_StreamMutex;

#endif
	};
}

#endif // ZZL_FILELOGGER_H

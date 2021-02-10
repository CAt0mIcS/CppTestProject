#pragma once

#include "RBaseLogger.h"
#include <iostream>
#include <cstring>


namespace At0::VulkanTesting
{
	class ConsoleLogger : public BaseLogger
	{
	public:
		ConsoleLogger() = default;

		void Flush() override
		{
			std::cout.flush();
			std::wcout.flush();
		}

		void Log(std::string_view message, LogMessageType type) override
		{
			char color[6];
			switch (type)
			{
			case LogMessageType::Trace: strcpy(color, "\033[39m"); break;
			case LogMessageType::Debug: strcpy(color, "\033[37m"); break;
			case LogMessageType::Information: strcpy(color, "\033[92m"); break;
			case LogMessageType::Warning: strcpy(color, "\033[33m"); break;
			case LogMessageType::Error: strcpy(color, "\033[91m"); break;
			case LogMessageType::Critical: strcpy(color, "\033[31m"); break;
			}

			std::scoped_lock lock(m_CoutMutex);
			std::cout << color << message << "\033[39m" << '\n';
		}

	private:
		std::mutex m_CoutMutex;
	};
}  // namespace At0::VulkanTesting

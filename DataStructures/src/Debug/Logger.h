#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <ctime>
#include <sstream>

#define QRD_LOG_ACTIVE 1


namespace QRD
{

	class Logger
	{
	public:
		static void Init(const std::string& filepath);

		template<typename T>
		static void Log(const T& message, const std::string& filepath);
	};

	template<typename T>
	void Logger::Log(const T& message, const std::string& filepath)
	{
		std::ofstream stream(filepath, std::ios_base::app);
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		std::stringstream ss;
		ss << message;

		stream << '[' << now->tm_hour << ':' << now->tm_min << ':' << round(now->tm_sec) << "]: " << ss.str() << '\n';

		stream.close();
	}

}


#if QRD_LOG_ACTIVE
	template<typename T>
	inline void QRD_LOG(const T& text)
	{
		QRD::Logger::Log((*(T*)&text), "D:\\dev\\Cpp\\Projects\\DataStructures\\log.txt");
	}
	#define QRD_LOG_CLEAR() QRD::Logger::Init("D:\\dev\\Cpp\\Projects\\DataStructures\\log.txt")
#else
	template<typename T>
	inline void QRD_LOG(const T& text) {}
	//#define QRD_LOG(text)
	#define QRD_LOG_CLEAR()
#endif
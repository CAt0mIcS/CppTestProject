#include "rldpch.h"
#include "RlDebug/Instrumentor.h"

#if RL_ENABLE_PROFILING

#include "ReyalLogger.h"


namespace At0::Reyal::Debug
{
	void Instrumentor::BeginSession(const std::string_view name, const std::string_view filepath)
	{
		std::scoped_lock lock(m_Mutex);

		// Close session if one already exists
		if (m_CurrentSession)
		{
			if (RL_FL_LOGGER_OPEN())
				RL_LOG_DEBUG("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
			InternalEndSession();
		}

		m_Writer.open(filepath.data());
		if (m_Writer.is_open())
		{
			m_CurrentSession = new Session{name.data()};
			WriteHeader();
		}
		else
		{
			if (RL_FL_LOGGER_OPEN())
				RL_LOG_DEBUG("Instrumentor could not open results file '{0}'.", filepath);
		}
	}

	void Instrumentor::EndSession()
	{
		std::scoped_lock lock(m_Mutex);
		InternalEndSession();
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		std::ostringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.ThreadID << ",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		std::scoped_lock lock(m_Mutex);
		if (m_CurrentSession)
		{
			m_Writer << json.str();
			m_Writer.flush();
		}
		else
		{
			if (RL_FL_LOGGER_OPEN())
				RL_LOG_DEBUG("[Instrumentor] Timer with function signature '{0}' took '{1}'us", result.Name, result.ElapsedTime.count());
		}
	}

	Instrumentor& Instrumentor::Get()
	{
		static Instrumentor instance;
		return instance;
	}

	Instrumentor::Instrumentor()
		: m_CurrentSession(nullptr) 
	{

	}

	Instrumentor::~Instrumentor()
	{
		EndSession();
	}

	void Instrumentor::WriteHeader()
	{
		m_Writer << "{\"otherData\": {},\"traceEvents\":[{}";
		m_Writer.flush();
	}

	void Instrumentor::WriteFooter()
	{
		m_Writer << "]}";
		m_Writer.flush();
	}

	void Instrumentor::InternalEndSession()
	{
		if (m_CurrentSession)
		{
			WriteFooter();
			m_Writer.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}

	/// <summary>
	/// Timer
	/// </summary>
	Timer::Timer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	Timer::~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Timer::Stop()
	{
		auto endPoint = std::chrono::steady_clock::now();
		auto highResStart = std::chrono::duration<double, std::micro>{ m_StartTimepoint.time_since_epoch() };
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

		Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
		m_Stopped = true;
	}
}

#endif
#include "pch.h"
#include "Sandbox.h"


#include <Reyal/Reyal.h>

#include <filesystem>
#include <Windows.h>

namespace At0
{
	Sandbox::Sandbox()
	{
		//RL_PROFILE_FUNCTION();
		
		m_MainWindow.InitRenderer3D();
		m_MainWindow.SetTitle(L"Zeal");
		m_MainWindow.Show();
		
		// Test loading in layers
		using LayerCreateFunc = Reyal::Layer* (*)();
		const std::wstring ending = L".dll";
		
		#if defined(_DEBUG) || defined(DEBUG)
			std::string outStr = "../../bin/Debug-Windows";
		#else
			std::string outStr = "../../bin/Release-Windows";
		#endif
		
		for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(outStr))
		{
			std::wstring_view path = dirEntry.path().native();
		
			if (path.size() < ending.size() || path.compare(path.size() - ending.size(), ending.size(), ending) != 0)
				continue;
		
			HMODULE hDll = LoadLibrary(path.data());
			if (!hDll || hDll == INVALID_HANDLE_VALUE)
			{
				MessageBox(NULL, L"Error", L"Cannot find dll", MB_OK);
				return;
			}
		
			LayerCreateFunc layerCreateFunc = (LayerCreateFunc)GetProcAddress(hDll, "CreateLayer");
			if (!layerCreateFunc)
			{
				continue;
			}
		
			PushLayer(layerCreateFunc());
		}

	}
	
	Sandbox::~Sandbox()
	{
		//RL_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{
		//RL_PROFILE_FUNCTION();
	}

}



/////////////////////////////////////////////////////////////////////////////////////////////////////
////////// Entry Point (TODO) ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Reyal/Application.h"
#include <RlDebug/ReyalLogger.h>
#include <RlDebug/Instrumentor.h>
#include <RlUtil/Exception.h>

#include <signal.h>


/// <summary>
/// Handles any unexpected signals, logs the signal and closes the logger
/// </summary>
/// <param name="signum">Is the received signal</param>
void SignalHandler(int signum)
{
	RL_LOG_CRITICAL("Signal '{0}' received, terminating program", signum);

	// Note: We do not need to do this as the destructor does it automatically, 
	// but it's good to have it here
	RL_LOG_END();

	// We need to do this though
	RL_PROFILE_END_SESSION();

	exit(signum);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	using namespace At0;

	RL_LOG_BEGIN("../../Zeal.log", Log::LogLevel::Trace);

	/// <summary>
	/// TODO:
	///		Util::SetSignals (TODO)
	/// </summary>
	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGTERM, SignalHandler);

	//TODO: Awake function (maybe)?

	#ifdef _DEBUG
	{
		AllocConsole();
		FILE* file;
		freopen_s(&file, "CONOUT$", "w", stdout);
	}
	#endif

	try
	{
		RL_PROFILE_BEGIN_SESSION("Startup", "Profiling/Profile-Startup.json");
		Reyal::CreateApplication();
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Runtime", "Profiling/Profile-Runtime.json");
		int exitCode = Reyal::Application::Get().Run();
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Shutdown", "Profiling/Profile-Shutdown.json");
		Reyal::Application::Destroy();
		RL_PROFILE_END_SESSION();

#ifdef _DEBUG
		FreeConsole();
#endif
		RL_LOG_END();
		return exitCode;
	}
	// TODO: Custom Window for errors
	catch (At0::Reyal::Exception& e)
	{
		MessageBoxA(NULL, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Standard Exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(NULL, "An unknown exception occurred", "Unknown Exception", MB_OK | MB_ICONERROR);
	}

	return -1;
}


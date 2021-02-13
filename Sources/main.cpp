#include "Window.h"
#include "Utils/RException.h"
#include "Utils/RLogger.h"
#include "Graphics/Graphics.h"

#include <chrono>

using namespace At0::VulkanTesting;


int main()
{
	try
	{
		Log::SetLogLevel(LogLevel::Trace);
		Log::Open("build/VulkanTesting.log");

		// Create window
		Window::Create();

		// Create graphics
		Graphics::Create();

		auto prevTime = std::chrono::high_resolution_clock::now();
		while (Window::Get().Update())
		{
			// Calculate frametime in ms
			auto tNow = std::chrono::high_resolution_clock::now();
			double diff = (double)((tNow - prevTime).count()) / 1000.0 / 1000.0;
			prevTime = tNow;

			// std::cout << "Frametime: " << diff << '\n';

			Graphics::Get().Update((float)diff / 1000.0f);
		}

		Graphics::Destroy();
	}
	catch (Exception& e)
	{
		Log::Critical("[{0}]: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Log::Critical("[{0}]: {1}", "Standard Exception", e.what());
	}
	catch (...)
	{
		Log::Critical("[{0}]", "Unknown Exception");
	}
}

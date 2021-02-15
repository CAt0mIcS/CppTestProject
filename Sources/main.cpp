#include "Window.h"
#include "Utils/RException.h"
#include "Utils/RLogger.h"
#include "Graphics/Graphics.h"

#include <chrono>
#include <sstream>

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
		auto startSecTime = std::chrono::high_resolution_clock::now();
		while (Window::Get().Update())
		{
			// Calculate frametime in ms
			auto tNow = std::chrono::high_resolution_clock::now();
			float dt = ((float)(tNow - prevTime).count()) / 1000.0f / 1000.0f / 1000.0f;
			prevTime = tNow;

			Graphics::Get().Update(dt);

			// Update frametime and fps info every 0.5s
			if (std::chrono::duration_cast<std::chrono::milliseconds>(tNow - startSecTime)
					.count() >= 500)
			{
				uint32_t currFPS = 996.0f / (dt * 1000.0f);
				std::ostringstream oss;
				oss << "Frametime: " << dt << "s"
					<< ", FPS: " << currFPS;
				Window::Get().SetTitle(oss.str());

				startSecTime = tNow;
			}
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

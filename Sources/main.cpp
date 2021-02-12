﻿#include "Window.h"
#include "Utils/RException.h"
#include "Utils/RLogger.h"
#include "Graphics/Graphics.h"

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
		Graphics::Get();

		std::thread updateGraphics([]() {
			while (Window::Get().IsOpen())
			{
				Graphics::Get().Update();
			}
		});

		while (Window::Get().Update())
		{
		}

		if (updateGraphics.joinable())
			updateGraphics.join();
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

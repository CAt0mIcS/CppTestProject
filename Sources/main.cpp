#include "Window.h"
#include "Utils/RLogger.h"
#include "Graphics/Graphics.h"

using namespace At0::VulkanTesting;


int main()
{
	Log::Open("../VulkanTesting.log");
	Window::Create();
	Graphics::Create();

	while (Window::Get().Update())
	{
	}
}
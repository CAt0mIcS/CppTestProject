#include "Window.h"

using namespace At0::VulkanTesting;


int main()
{
	Window::Create();

	while (Window::Get().Update())
	{
	}
}
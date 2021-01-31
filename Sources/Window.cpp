#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>


namespace At0::VulkanTesting
{
	void Window::Create()
	{
		if (!s_Instance)
			s_Instance = std::make_unique<Window>();
	}

	Window::Window()
	{
	}
}
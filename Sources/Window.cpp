#include "Window.h"

#include "Utils/RAssert.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace At0::VulkanTesting
{
	void Window::Create()
	{
		if (!s_Instance)
			s_Instance = std::make_unique<Window>();
	}

	bool Window::Update()
	{
		glfwPollEvents();

		return !glfwWindowShouldClose(m_hWnd);
	}

	Window::Window()
	{
		RAY_MEXPECTS(glfwInit(), "Failed to initialize GLFW.");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_hWnd = glfwCreateWindow(960, 540, "Vulkan Testing", nullptr, nullptr);
		glfwShowWindow(m_hWnd);
	}
}
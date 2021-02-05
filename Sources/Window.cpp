#include "Window.h"

#include "Utils/RAssert.h"
#include "Utils/RException.h"

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

	std::pair<const char**, uint32_t> Window::GetInstanceExtensions() const
	{
		uint32_t count = 0;
		const char** data = glfwGetRequiredInstanceExtensions(&count);
		return std::make_pair(data, count);
	}

	void Window::CreateSurface(
		VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface)
	{
		RAY_VK_THROW_FAILED(glfwCreateWindowSurface(instance, m_hWnd, allocator, surface),
			"Failed to create glfw window surface.");
	}

	Window::Window()
	{
		int success = glfwInit();
		RAY_MEXPECTS(success, "Failed to initialize GLFW.");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_hWnd = glfwCreateWindow(960, 540, "Vulkan Testing", nullptr, nullptr);
		glfwShowWindow(m_hWnd);
	}
}  // namespace At0::VulkanTesting

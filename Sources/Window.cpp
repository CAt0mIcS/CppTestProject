#include "Window.h"

#include "Utils/RAssert.h"
#include "Utils/RException.h"
#include "Graphics/Graphics.h"

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

	bool Window::IsOpen() const { return !glfwWindowShouldClose(m_hWnd); }

	void Window::WaitForEvents() const { glfwWaitEvents(); }

	bool Window::CursorEnabled() const
	{
		return glfwGetInputMode(m_hWnd, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}

	void Window::EnableCursor() const { glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	void Window::DisableCursor() const
	{
		glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	void Window::GetSize(int* width, int* height) const
	{
		glfwGetWindowSize(m_hWnd, width, height);
	}

	void Window::GetFramebufferSize(int* width, int* height) const
	{
		glfwGetFramebufferSize(m_hWnd, width, height);
	}

	Window::Window()
	{
		int success = glfwInit();
		RAY_MEXPECTS(success, "Failed to initialize GLFW.");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_hWnd = glfwCreateWindow(960, 540, "Vulkan Testing", nullptr, nullptr);
		glfwShowWindow(m_hWnd);


		glfwSetWindowSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height) {
			Graphics::Get().m_FramebufferResized = true;
		});

		glfwSetKeyCallback(m_hWnd, [](GLFWwindow*, int key, int, int action, int) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				if (Window::Get().CursorEnabled())
					Window::Get().DisableCursor();
				else
					Window::Get().EnableCursor();
			}

			if (Graphics::Get().SceneCamera.Type == Camera::FirstPerson)
			{
				switch (action)
				{
				case GLFW_PRESS:
					switch (key)
					{
					case GLFW_KEY_W: Graphics::Get().SceneCamera.Keys.Forward = true; break;
					case GLFW_KEY_S: Graphics::Get().SceneCamera.Keys.Backward = true; break;
					case GLFW_KEY_A: Graphics::Get().SceneCamera.Keys.Left = true; break;
					case GLFW_KEY_D: Graphics::Get().SceneCamera.Keys.Right = true; break;
					case GLFW_KEY_SPACE: Graphics::Get().SceneCamera.Keys.Up = true; break;
					case GLFW_KEY_LEFT_CONTROL: Graphics::Get().SceneCamera.Keys.Down = true; break;
					}
					break;
				case GLFW_RELEASE:
					switch (key)
					{
					case GLFW_KEY_W: Graphics::Get().SceneCamera.Keys.Forward = false; break;
					case GLFW_KEY_S: Graphics::Get().SceneCamera.Keys.Backward = false; break;
					case GLFW_KEY_A: Graphics::Get().SceneCamera.Keys.Left = false; break;
					case GLFW_KEY_D: Graphics::Get().SceneCamera.Keys.Right = false; break;
					case GLFW_KEY_SPACE: Graphics::Get().SceneCamera.Keys.Up = false; break;
					case GLFW_KEY_LEFT_CONTROL:
						Graphics::Get().SceneCamera.Keys.Down = false;
						break;
					}
					break;
				}
			}
		});

		glfwSetScrollCallback(m_hWnd, [](GLFWwindow*, double scrollX, double scrollY) {
			// Graphics::Get().SceneCamera.Translate(
			//	glm::vec3(0.0f, 0.0f, (float)wheelDelta * 0.005f));
		});

		glfwSetCursorPosCallback(m_hWnd, [](GLFWwindow*, double xPos, double yPos) {
			int dx = (int)Window::Get().m_MousePos.x - xPos;
			int dy = (int)Window::Get().m_MousePos.y - yPos;

			Camera& camera = Graphics::Get().SceneCamera;
			if (!Window::Get().CursorEnabled())
				camera.Rotate(glm::vec3(dy * camera.RotationSpeed * Graphics::Get().GetDelta(),
					-dx * camera.RotationSpeed * Graphics::Get().GetDelta(), 0.0f));

			Window::Get().m_MousePos = { (float)xPos, (float)yPos };
		});
	}  // namespace At0::VulkanTesting
}  // namespace At0::VulkanTesting

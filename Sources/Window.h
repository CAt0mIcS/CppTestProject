#pragma once

#include <memory>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GLFWwindow;

namespace At0::VulkanTesting
{
	class Window
	{
		friend std::unique_ptr<Window> std::make_unique<Window>();

	public:
		static void Create();
		static Window& Get() { return *s_Instance; }

		bool Update();
		bool IsOpen() const;
		void WaitForEvents() const;

		bool CursorEnabled() const;
		void EnableCursor() const;
		void DisableCursor() const;

		std::pair<const char**, uint32_t> GetInstanceExtensions() const;
		void CreateSurface(
			VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);
		void GetSize(int* width, int* height) const;
		void GetFramebufferSize(int* width, int* height) const;

	private:
		Window();

	private:
		inline static std::unique_ptr<Window> s_Instance = nullptr;
		GLFWwindow* m_hWnd;

		glm::vec2 m_MousePos{};
	};
}  // namespace At0::VulkanTesting

#pragma once

#include <memory>

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

		std::pair<const char**, uint32_t> GetInstanceExtensions() const;

	private:
		Window();

	private:
		inline static std::unique_ptr<Window> s_Instance = nullptr;
		GLFWwindow* m_hWnd;
	};
}  // namespace At0::VulkanTesting

#pragma once

#include <memory>


namespace At0::VulkanTesting
{
	class Window
	{
	public:
		void Create();

		static Window& Get() { return *s_Instance; }

	private:
		Window();

	private:
		inline static std::unique_ptr<Window> s_Instance = nullptr;
	};
}
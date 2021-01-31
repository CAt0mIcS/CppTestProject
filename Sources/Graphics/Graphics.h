#pragma once

#include <memory>


namespace At0::VulkanTesting
{
	class VulkanInstance;
	class PhysicalDevice;
	class LogicalDevice;

	class Graphics
	{
		friend std::unique_ptr<Graphics> std::make_unique<Graphics>();

	public:
		static void Create();
		static Graphics& Get() { return *s_Instance; }

	private:
		inline static std::unique_ptr<Graphics> s_Instance = nullptr;
	};
}  // namespace At0::VulkanTesting
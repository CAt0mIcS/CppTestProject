#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class LogicalDevice;

	class CommandPool
	{
	public:
		CommandPool(const LogicalDevice* device);
		~CommandPool();

		operator const VkCommandPool&() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool;
		const LogicalDevice& m_LogicalDevice;
	};
}  // namespace At0::VulkanTesting
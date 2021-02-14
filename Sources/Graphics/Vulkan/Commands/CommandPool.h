#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class CommandPool
	{
	public:
		CommandPool();
		~CommandPool();

		operator const VkCommandPool&() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool;
	};
}  // namespace At0::VulkanTesting

#pragma once

#include <vulkan/vulkan_core.h>
#include "Base.h"


namespace At0::VulkanTesting
{
	class CommandBuffer
	{
	public:
		CommandBuffer();
		~CommandBuffer();

		void Begin(VkCommandBufferUsageFlags usage = 0);
		void End();

		operator const VkCommandBuffer&() const { return m_CommandBuffer; }

	private:
		VkCommandBuffer m_CommandBuffer;
		bool m_Running = false;
	};
}  // namespace At0::VulkanTesting

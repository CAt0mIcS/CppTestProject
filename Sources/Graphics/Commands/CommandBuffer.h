#pragma once

#include <vulkan/vulkan_core.h>
#include <memory>


namespace At0::VulkanTesting
{
	class CommandPool;
	class LogicalDevice;

	class CommandBuffer
	{
	public:
		CommandBuffer(const LogicalDevice* device,
			VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT,
			VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer();

		void Begin();
		void End();

	private:
		const CommandPool& m_CommandPool;
		const LogicalDevice& m_LogicalDevice;

		VkCommandBuffer m_CommandBuffer;
		VkQueueFlagBits m_QueueType;
		bool m_Running = false;
	};
}  // namespace At0::VulkanTesting
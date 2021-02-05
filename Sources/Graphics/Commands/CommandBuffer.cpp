#include "CommandBuffer.h"
#include "../Graphics.h"
#include "../LogicalDevice.h"
#include "../../Utils/RException.h"
#include "CommandPool.h"


namespace At0::VulkanTesting
{
	CommandBuffer::CommandBuffer(
		const LogicalDevice* device, VkQueueFlagBits queueType, VkCommandBufferLevel bufferLevel)
		: m_LogicalDevice(*device), m_CommandPool(Graphics::Get().GetCommandPool()),
		  m_QueueType(queueType)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;
		RAY_VK_THROW_FAILED(vkAllocateCommandBuffers(m_LogicalDevice, nullptr, &m_CommandBuffer),
			"Failed to allocate command buffers.");
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, nullptr);
	}

	void CommandBuffer::Begin()
	{
		if (m_Running)
			return;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		RAY_VK_THROW_FAILED(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo),
			"Failed to begin command buffer recording.");
		m_Running = true;
	}

	void CommandBuffer::End()
	{
		if (!m_Running)
			return;

		RAY_VK_THROW_FAILED(
			vkEndCommandBuffer(m_CommandBuffer), "Failed to end command buffer recording.");
	}
}  // namespace At0::VulkanTesting
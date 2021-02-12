#include "CommandBuffer.h"
#include "../Graphics.h"
#include "../LogicalDevice.h"
#include "../../Utils/RException.h"
#include "CommandPool.h"


namespace At0::VulkanTesting
{
	CommandBuffer::CommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = Graphics::Get().GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		RAY_VK_THROW_FAILED(vkAllocateCommandBuffers(
								Graphics::Get().GetLogicalDevice(), &allocInfo, &m_CommandBuffer),
			"Failed to allocate command buffers.");
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(Graphics::Get().GetLogicalDevice(), Graphics::Get().GetCommandPool(),
			1, &m_CommandBuffer);
	}

	void CommandBuffer::Begin(VkCommandBufferUsageFlags usage)
	{
		if (m_Running)
			return;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
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

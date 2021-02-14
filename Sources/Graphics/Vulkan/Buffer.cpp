#include "Buffer.h"
#include "Graphics/Graphics.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	void Buffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		RAY_VK_THROW_FAILED(
			vkCreateBuffer(Graphics::Get().GetLogicalDevice(), &bufferInfo, nullptr, &buffer),
			"Failed to create buffer.");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Graphics::Get().GetLogicalDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		RAY_VK_THROW_FAILED(vkAllocateMemory(Graphics::Get().GetLogicalDevice(), &allocInfo,
								nullptr, &bufferMemory),
			"Failed to allocate buffer memory");

		vkBindBufferMemory(Graphics::Get().GetLogicalDevice(), buffer, bufferMemory, 0);
	}

	uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProps =
			Graphics::Get().GetPhysicalDevice().GetMemoryProperties();

		for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
		{
			if (typeFilter & (1 << i) &&
				(memProps.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		RAY_THROW_RUNTIME("Failed to find suitable memory type");
	}

	void Buffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		// VT_TODO: Create separate command pool for short-lived command buffers

		CommandBuffer commandBuffer;
		commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		commandBuffer.End();

		VkCommandBuffer cmdBuff = commandBuffer;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		// VK_TODO: Check which queue is faster and enable multithreading by using fences
		// instead of vkQueueWaitIdle
		vkQueueSubmit(
			Graphics::Get().GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Graphics::Get().GetLogicalDevice().GetGraphicsQueue());
	}
}  // namespace At0::VulkanTesting

#include "pch.h"
#include "Buffer.h"
#include "Graphics/Graphics.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		const void* data)
		: m_Size(size)
	{
		std::array queueFamily = { Graphics::Get().GetLogicalDevice().GetGraphicsFamily(),
			Graphics::Get().GetLogicalDevice().GetPresentFamily(),
			Graphics::Get().GetLogicalDevice().GetComputeFamily() };

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_Size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = (uint32_t)queueFamily.size();
		bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();

		RAY_VK_THROW_FAILED(vkCreateBuffer(Graphics::Get().GetLogicalDevice(), &bufferCreateInfo,
								nullptr, &m_Buffer),
			"Failed to create buffer");

		// Create the memory backing up the buffer handdle
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(
			Graphics::Get().GetLogicalDevice(), m_Buffer, &memoryRequirements);

		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllocInfo.allocationSize = memoryRequirements.size;
		memAllocInfo.memoryTypeIndex =
			FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		RAY_VK_THROW_FAILED(vkAllocateMemory(Graphics::Get().GetLogicalDevice(), &memAllocInfo,
								nullptr, &m_BufferMemory),
			"Failed to allocate buffer memory");

		// Copy data if data ptr has been passed to the function
		if (data)
		{
			void* mapped;
			MapMemory(&mapped);
			memcpy(mapped, data, m_Size);

			// If host coherency hasn't been requested, do a manual flush to make writes visible
			if ((properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
			{
				VkMappedMemoryRange mappedMemoryRange{};
				mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				mappedMemoryRange.memory = m_BufferMemory;
				mappedMemoryRange.offset = 0;
				mappedMemoryRange.size = m_Size;
				RAY_VK_THROW_FAILED(vkFlushMappedMemoryRanges(
										Graphics::Get().GetLogicalDevice(), 1, &mappedMemoryRange),
					"Failed to flush memory");
			}

			UnmapMemory();
		}

		// Attach the memory to the buffer object
		RAY_VK_THROW_FAILED(
			vkBindBufferMemory(Graphics::Get().GetLogicalDevice(), m_Buffer, m_BufferMemory, 0),
			"Failed to map buffer to buffer memory");
	}

	Buffer::~Buffer()
	{
		vkDestroyBuffer(Graphics::Get().GetLogicalDevice(), m_Buffer, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory, nullptr);
	}

	void Buffer::MapMemory(void** data) const
	{
		RAY_VK_THROW_FAILED(
			vkMapMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory, 0, m_Size, 0, data),
			"Failed to map memory");
	}

	void Buffer::UnmapMemory() const
	{
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory);
	}

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

	Buffer::Buffer(VkDeviceSize size) : m_Size(size) {}

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

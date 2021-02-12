#pragma once

#include <vulkan/vulkan_core.h>

namespace At0::VulkanTesting
{
	class Buffer
	{
	protected:
		Buffer() = default;
		// virtual ~Buffer();

		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
	};
}  // namespace At0::VulkanTesting

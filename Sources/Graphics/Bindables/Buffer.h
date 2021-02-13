#pragma once

#include <vulkan/vulkan_core.h>

namespace At0::VulkanTesting
{
	class Buffer
	{
	public:
		operator const VkBuffer&() const { return m_Buffer; }

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	protected:
		Buffer(VkDeviceSize bufferSize, const void* const bufferData, VkBufferUsageFlags usage);
		virtual ~Buffer();

		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
	};
}  // namespace At0::VulkanTesting

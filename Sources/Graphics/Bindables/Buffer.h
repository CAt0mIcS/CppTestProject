#pragma once

#include <vulkan/vulkan_core.h>

namespace At0::VulkanTesting
{
	class Buffer
	{
	public:
		operator const VkBuffer&() const { return m_Buffer; }

	protected:
		Buffer(VkDeviceSize bufferSize, const void* const bufferData);
		virtual ~Buffer();

		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
	};
}  // namespace At0::VulkanTesting

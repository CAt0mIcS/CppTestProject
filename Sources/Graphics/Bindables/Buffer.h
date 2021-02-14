#pragma once

#include <vulkan/vulkan_core.h>

namespace At0::VulkanTesting
{
	class CommandBuffer;

	class Buffer
	{
	public:
		operator const VkBuffer&() const { return m_Buffer; }

		virtual void Bind(CommandBuffer& cmdBuff) = 0;

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	protected:
		Buffer(VkDeviceSize bufferSize, const void* const bufferData, VkBufferUsageFlags usage);
		virtual ~Buffer();

		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	protected:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
	};
}  // namespace At0::VulkanTesting

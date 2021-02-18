#pragma once

#include <vulkan/vulkan_core.h>
#include "Base.h"


namespace At0::VulkanTesting
{
	class CommandBuffer;

	class Buffer
	{
	public:
		enum class Status
		{
			Reset,
			Changed,
			Normal
		};

	public:
		/**
		 * Creates a new buffer with optional data.
		 * @param size Size of the buffer in bytes.
		 * @param usage Usage flag bitmask for the buffer (i.e. index, vertex, uniform buffer).
		 * @param properties Memory properties for this buffer (i.e. device local, host visible,
		 * coherent).
		 * @param data Pointer to the data that should be copied to the buffer after creation
		 * (optional, if not set, no data is copied over).
		 */
		Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			const void* data = nullptr);
		virtual ~Buffer();

		VkDeviceSize GetSize() const { return m_Size; }

		void MapMemory(void** data) const;
		void UnmapMemory() const;
		operator const VkBuffer&() const { return m_Buffer; }

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


	protected:
		Buffer(VkDeviceSize size);

	protected:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		VkDeviceSize m_Size;
	};
}  // namespace At0::VulkanTesting

#include "VertexBuffer.h"
#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex> vertices)
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(
			Graphics::Get().GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory);

		CreateBuffer(bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_BufferMemory);

		CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

		vkDestroyBuffer(Graphics::Get().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	VertexBuffer::~VertexBuffer()
	{
		vkDestroyBuffer(Graphics::Get().GetLogicalDevice(), m_VertexBuffer, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory, nullptr);
	}
}  // namespace At0::VulkanTesting
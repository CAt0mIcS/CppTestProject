#include "pch.h"
#include "VertexBuffer.h"

#include "Graphics/Graphics.h"
#include "LogicalDevice.h"
#include "Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	VertexBuffer::VertexBuffer(const std::string_view tag, const std::vector<Vertex>& vertices)
		: Buffer(sizeof(vertices[0]) * vertices.size())
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory, 0, m_Size, 0, &data);
		memcpy(data, vertices.data(), (size_t)m_Size);
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory);

		CreateBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_BufferMemory);

		CopyBuffer(stagingBuffer, m_Buffer, m_Size);

		vkDestroyBuffer(Graphics::Get().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void VertexBuffer::Bind(const CommandBuffer& cmdBuff)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(cmdBuff, 0, 1, &m_Buffer, offsets);
	}

	std::string VertexBuffer::GetUID(std::string_view tag, const std::vector<Vertex>& indices)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag.data();
	}
}  // namespace At0::VulkanTesting

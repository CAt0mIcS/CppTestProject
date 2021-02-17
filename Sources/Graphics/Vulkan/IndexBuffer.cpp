#include "pch.h"
#include "IndexBuffer.h"
#include "Utils/RAssert.h"

#include "Graphics/Graphics.h"
#include "LogicalDevice.h"
#include "Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	IndexBuffer::IndexBuffer(const std::vector<Type>& indices, std::string_view tag)
		: Buffer(sizeof(indices[0]) * indices.size()), m_NumIndices((uint32_t)indices.size()),
		  m_Tag(tag)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory, 0, m_Size, 0, &data);
		memcpy(data, indices.data(), (size_t)m_Size);
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory);

		CreateBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_BufferMemory);

		CopyBuffer(stagingBuffer, m_Buffer, m_Size);

		vkDestroyBuffer(Graphics::Get().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void IndexBuffer::Bind(const CommandBuffer& cmdBuff)
	{
		vkCmdBindIndexBuffer(cmdBuff, m_Buffer, 0, VK_INDEX_TYPE_UINT16);
	}

	uint32_t IndexBuffer::GetNumIndices() const
	{
		RAY_MEXPECTS(m_NumIndices != 0, "Didn't set number of indices in the index buffer");
		return m_NumIndices;
	}

	std::string IndexBuffer::GetUID(const std::vector<uint16_t>& indices, std::string_view tag)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag.data();
	}
}  // namespace At0::VulkanTesting

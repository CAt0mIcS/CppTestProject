#include "UniformBuffer.h"

#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	UniformBuffer::UniformBuffer()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer,
			m_BufferMemory);
	}

	void UniformBuffer::Bind(CommandBuffer& cmdBuff) {}

	void UniformBuffer::Update(const UniformBufferObject& ubo)
	{
		void* data;
		vkMapMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory);
	}
}  // namespace At0::VulkanTesting

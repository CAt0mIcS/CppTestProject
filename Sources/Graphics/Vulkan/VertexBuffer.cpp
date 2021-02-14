#include "VertexBuffer.h"
#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex> vertices)
		: Buffer(sizeof(vertices[0]) * vertices.size(), vertices.data(),
			  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
	{
	}

	void VertexBuffer::Bind(CommandBuffer& cmdBuff)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(cmdBuff, 0, 1, &m_Buffer, offsets);
	}
}  // namespace At0::VulkanTesting

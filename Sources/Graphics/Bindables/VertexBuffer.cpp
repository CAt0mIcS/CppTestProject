#include "VertexBuffer.h"
#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex> vertices)
		: Buffer(sizeof(vertices[0]) * vertices.size(), vertices.data(),
			  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
	{
	}
}  // namespace At0::VulkanTesting
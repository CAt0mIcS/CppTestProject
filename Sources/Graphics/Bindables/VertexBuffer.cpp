#include "VertexBuffer.h"
#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex> vertices)
		: Buffer(sizeof(vertices[0]) * vertices.size(), vertices.data())
	{
	}
}  // namespace At0::VulkanTesting
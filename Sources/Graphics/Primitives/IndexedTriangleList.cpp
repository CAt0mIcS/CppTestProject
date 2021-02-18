#include "pch.h"
#include "IndexedTriangleList.h"


namespace At0::VulkanTesting
{
	IndexedTriangleList::IndexedTriangleList(
		VertexInput input, std::vector<IndexBuffer::Type> indices)
		: m_Vertices(std::move(input)), m_Indices(std::move(indices))
	{
	}

	IndexedTriangleList IndexedTriangleList::UVSphere(VertexLayout layout)
	{
		VertexInput vertexInput(std::move(layout));
		std::vector<IndexBuffer::Type> indices;

		return { vertexInput, indices };
	}
}  // namespace At0::VulkanTesting

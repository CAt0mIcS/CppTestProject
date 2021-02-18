#pragma once

#include <vector>

#include "Vertex.h"
#include "Graphics/Vulkan/IndexBuffer.h"


namespace At0::VulkanTesting
{
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList(VertexInput input, std::vector<IndexBuffer::Type> indices);

		static IndexedTriangleList UVSphere(VertexLayout layout);

		const VertexInput& GetVertexInput() const { return m_Vertices; }
		const std::vector<IndexBuffer::Type>& GetIndices() const { return m_Indices; }

	private:
		VertexInput m_Vertices;
		std::vector<IndexBuffer::Type> m_Indices;
	};
}  // namespace At0::VulkanTesting

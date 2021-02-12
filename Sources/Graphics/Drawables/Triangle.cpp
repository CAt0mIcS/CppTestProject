#include "Triangle.h"

#include "Graphics/Bindables/VertexBuffer.h"

namespace At0::VulkanTesting
{
	Triangle::Triangle()
	{
		if (!s_VertexBuffer)
		{
			// clang-format off
			std::vector<Vertex> vertices 
			{ 
				{ {  0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
				{ {  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
				{ { -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } } 
			};
			// clang-format on
			s_VertexBuffer = std::make_unique<VertexBuffer>(vertices);
		}
	}

}  // namespace At0::VulkanTesting
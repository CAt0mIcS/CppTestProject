#include "Cube.h"


namespace At0::VulkanTesting
{
	Cube::Cube()
	{
		if (!s_VertexBuffer)
		{
			// clang-format off
			std::vector<Vertex> vertices
			{
				{{ -0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f }},
				{{  0.5f, -0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f }},
				{{ -0.5f,  0.5f, -0.5f}, { 0.0f, 0.0f, 1.0f }},
				{{  0.5f,  0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }},
				{{ -0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f, 1.0f }},
				{{  0.5f, -0.5f,  0.5f}, { 0.0f, 1.0f, 1.0f }},
				{{ -0.5f,  0.5f,  0.5f}, { 1.0f, 1.0f, 1.0f }},
				{{  0.5f,  0.5f,  0.5f}, { 0.0f, 0.0f, 0.0f }},
			};
			s_VertexBuffer = std::make_unique<VertexBuffer>(vertices);

			std::vector<uint16_t> indices
			{
				0, 2, 1,  2, 3, 1,
				1, 3, 5,  3, 7, 5,
				2, 6, 3,  3, 6, 7,
				4, 5, 7,  4, 7, 6,
				0, 4, 2,  2, 4, 6,
				0, 1, 4,  1, 5, 4
			};
			// clang-format on
			s_IndexBuffer = std::make_unique<IndexBuffer>(indices);
		}
	}
}  // namespace At0::VulkanTesting

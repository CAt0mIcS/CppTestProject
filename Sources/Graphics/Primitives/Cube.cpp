#include "Cube.h"
#include "Graphics/Core/Codex.h"

#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	Cube::Cube()
	{
		// clang-format off
		std::vector<Vertex> vertices
		{
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
			{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
			{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } },
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } },
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f } },
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
			{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 0.0f } },
		};
		EmplaceBindable(Codex::Resolve<VertexBuffer>("Cube", vertices));

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
		EmplaceBindable(Codex::Resolve<IndexBuffer>("CubeIndices", indices));

		EmplaceBindable(Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderpass(),
			"Resources/Shaders/VertexShader.vert.spv",
			"Resources/Shaders/FragmentShader.frag.spv"));

		Drawable::InternalInit();
	}
}  // namespace At0::VulkanTesting

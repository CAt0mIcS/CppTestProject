#include "Square.h"

#include "Graphics/Core/Codex.h"
#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	Square::Square()
	{
		// clang-format off
			std::vector<Vertex> vertices
			{
				{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
				{ {  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f } },
				{ {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
				{ { -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
			};
		// clang-format on
		EmplaceBindable(Codex::Resolve<VertexBuffer>("Square", vertices));

		std::vector<uint16_t> indices{ 0, 1, 2, 2, 3, 0 };
		EmplaceBindable(Codex::Resolve<IndexBuffer>("012230", indices));

		EmplaceBindable(Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderpass(),
			"Resources/Shaders/VertexShader.vert.spv",
			"Resources/Shaders/FragmentShader.frag.spv"));

		Drawable::InternalInit();
	}
}  // namespace At0::VulkanTesting

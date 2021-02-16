#include "Triangle.h"
#include "Graphics/Core/Codex.h"

#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	Triangle::Triangle()
	{
		// clang-format off
		std::vector<Vertex> vertices 
		{ 
			{ { -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
			{ {  0.0f,  0.5f }, { 1.0f, 0.0f, 0.0f } }
		};
		// clang-format on

		std::vector<uint16_t> indices{ 0, 1, 2 };

		// The codex will check if the bindable already exists and return a "reference" to it
		// otherwise the codex will create the bindable and return a "reference" to it
		EmplaceBindable(Codex::Resolve<VertexBuffer>("Triangle", vertices));
		EmplaceBindable(Codex::Resolve<IndexBuffer>("012", indices));

		EmplaceBindable(Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderpass(),
			"Resources/Shaders/VertexShader.vert.spv",
			"Resources/Shaders/FragmentShader.frag.spv"));

		Drawable::InternalInit();
	}

}  // namespace At0::VulkanTesting

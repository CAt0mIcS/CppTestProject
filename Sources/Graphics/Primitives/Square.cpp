#include "Square.h"

#include "Graphics/Core/Codex.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/GraphicsPIpeline.h"
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

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
		EmplaceBindable(Codex::Resolve<IndexBuffer>("012230", indices));

		Ref<GraphicsPipeline> graphicsPipeline =
			Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderpass(),
				"Resources/Shaders/VertexShader.vert", "Resources/Shaders/FragmentShader.frag");
		EmplaceBindable(graphicsPipeline);
	}
}  // namespace At0::VulkanTesting

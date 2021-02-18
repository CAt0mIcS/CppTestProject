#include "Triangle.h"
#include "Graphics/Core/Codex.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"

#include "Vertex.h"


namespace At0::VulkanTesting
{
	Triangle::Triangle()
	{
		// clang-format off
		VertexLayout layout;
		layout.Append(VertexLayout::Position3D, VertexLayout::Float3Color);

		VertexInput vertexInput(layout);
		vertexInput.EmplaceBack(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f, -0.5f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.0f,  0.5f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
		// clang-format on

		// VK_TODO: 2D objects are only visible from one side because of backface culling
		// Specifying the vertices again in the reversed order works for now
		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 /*|*/, 2, 1, 0 };

		// The codex will check if the bindable already exists and return a "reference" to it
		// otherwise the codex will create the bindable and return a "reference" to it
		EmplaceBindable(Codex::Resolve<VertexBuffer>(vertexInput, "Triangle"));
		EmplaceBindable(Codex::Resolve<IndexBuffer>(indices, "012"));

		Ref<GraphicsPipeline> graphicsPipeline = Codex::Resolve<GraphicsPipeline>(
			layout, std::vector<std::string_view>{ "Resources/Shaders/VertexShader.vert",
						"Resources/Shaders/FragmentShader.frag" });
		EmplaceBindable(graphicsPipeline);
	}
}  // namespace At0::VulkanTesting

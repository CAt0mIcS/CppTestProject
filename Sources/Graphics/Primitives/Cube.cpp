#include "Cube.h"
#include "Graphics/Core/Codex.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"

#include "Vertex.h"


namespace At0::VulkanTesting
{
	Cube::Cube()
	{
		// clang-format off
		VertexLayout layout;
		layout.Append(VertexLayout::Position3D, VertexLayout::Float3Color);

		VertexInput vertexInput(layout);
		vertexInput.EmplaceBack(glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 1.0f, 1.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 1.0f, 0.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f, 1.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f, 0.0f, 0.0f });
		
		EmplaceBindable(Codex::Resolve<VertexBuffer>(vertexInput, "Cube"));

		std::vector<IndexBuffer::Type> indices
		{
			0, 2, 1,  2, 3, 1,
			1, 3, 5,  3, 7, 5,
			2, 6, 3,  3, 6, 7,
			4, 5, 7,  4, 7, 6,
			0, 4, 2,  2, 4, 6,
			0, 1, 4,  1, 5, 4
		};
		// clang-format on
		EmplaceBindable(Codex::Resolve<IndexBuffer>(indices, "CubeIndices"));

		Ref<GraphicsPipeline> graphicsPipeline = Codex::Resolve<GraphicsPipeline>(
			layout, std::vector<std::string_view>{ "Resources/Shaders/VertexShader.vert",
						"Resources/Shaders/FragmentShader.frag" });
		EmplaceBindable(graphicsPipeline);
	}
}  // namespace At0::VulkanTesting

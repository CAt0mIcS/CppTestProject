#include "Square.h"

#include "Graphics/Core/Codex.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/GraphicsPIpeline.h"
#include "Graphics/Graphics.h"

#include "Vertex.h"


namespace At0::VulkanTesting
{
	Square::Square()
	{

		// clang-format off
		VertexLayout layout;
		layout.Append(VertexLayout::Position3D, VertexLayout::Float3Color);

		VertexInput vertexInput(layout);
		vertexInput.EmplaceBack(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f, -0.5f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f,  0.5f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{ -0.5f,  0.5f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });

		// clang-format on
		EmplaceBindable(Codex::Resolve<VertexBuffer>(vertexInput, "Square"));

		// VK_TODO: 2D objects are only visible from one side because of backface culling
		// Specifying the vertices again in the reversed order works for now
		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 /*|*/, 0, 3, 2, 2, 1, 0 };
		EmplaceBindable(Codex::Resolve<IndexBuffer>(indices, "012230"));

		Ref<GraphicsPipeline> graphicsPipeline = Codex::Resolve<GraphicsPipeline>(
			layout, std::vector<std::string_view>{ "Resources/Shaders/VertexShader.vert",
						"Resources/Shaders/FragmentShader.frag" });
		EmplaceBindable(graphicsPipeline);
	}

	void Square::CmdDraw(const CommandBuffer& cmdBuff)
	{
		m_DescriptorsHandler.Push("UniformBufferObject", m_UniformHandler);
		m_DescriptorsHandler.Update(GetGraphicsPipeline());
		m_DescriptorsHandler.BindDescriptor(cmdBuff, GetGraphicsPipeline());

		Drawable::CmdDraw(cmdBuff);
	}
}  // namespace At0::VulkanTesting

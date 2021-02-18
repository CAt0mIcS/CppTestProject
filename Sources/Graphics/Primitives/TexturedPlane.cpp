#include "TexturedPlane.h"

#include "Graphics/Core/Codex.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/GraphicsPIpeline.h"
#include "Graphics/Graphics.h"

#include "Vertex.h"


namespace At0::VulkanTesting
{
	TexturedPlane::TexturedPlane() : m_Texture("Resources/Textures/gridbase.png")
	{

		// clang-format off
		VertexLayout layout;
		layout.Append(VertexLayout::Position3D, VertexLayout::Texture2D);

		VertexInput vertexInput(layout);
		vertexInput.EmplaceBack(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f, -0.5f, 0.0f }, glm::vec2{ 0.0f, 0.0f });
		vertexInput.EmplaceBack(glm::vec3{  0.5f,  0.5f, 0.0f }, glm::vec2{ 0.0f, 1.0f });
		vertexInput.EmplaceBack(glm::vec3{ -0.5f,  0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });

		// clang-format on
		EmplaceBindable(Codex::Resolve<VertexBuffer>(vertexInput, "TexturedPlane"));

		// VK_TODO: 2D objects are only visible from one side because of backface culling
		// Specifying the vertices again in the reversed order works for now
		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 /*|*/, 0, 3, 2, 2, 1, 0 };
		EmplaceBindable(Codex::Resolve<IndexBuffer>(indices, "012230"));

		Ref<GraphicsPipeline> graphicsPipeline = Codex::Resolve<GraphicsPipeline>(
			layout, std::vector<std::string_view>{ "Resources/Shaders/FlatTextureShader.vert",
						"Resources/Shaders/FlatTextureShader.frag" });
		EmplaceBindable(graphicsPipeline);
	}

	void TexturedPlane::CmdDraw(const CommandBuffer& cmdBuff)
	{
		m_DescriptorsHandler.Push("UniformBufferObject", m_UniformHandler);
		m_DescriptorsHandler.Push("samplerTex", m_Texture);
		m_DescriptorsHandler.Update(GetGraphicsPipeline());
		m_DescriptorsHandler.BindDescriptor(cmdBuff, GetGraphicsPipeline());

		Drawable::CmdDraw(cmdBuff);
	}
}  // namespace At0::VulkanTesting

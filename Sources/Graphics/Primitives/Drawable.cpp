#include "Drawable.h"
#include "Graphics/Graphics.h"

#include "Graphics/Vulkan/Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	void Drawable::CmdDraw(const CommandBuffer& cmdBuff)
	{
		// m_UniformBuffer->Bind(cmdBuff);
		for (Ref<Bindable>& bindable : m_Bindables)
		{
			bindable->Bind(cmdBuff);
		}

		m_DescriptorsHandler.Push("UniformBufferObject", m_UniformHandler);
		m_DescriptorsHandler.Update(GetGraphicsPipeline());
		m_DescriptorsHandler.BindDescriptor(cmdBuff, GetGraphicsPipeline());

		vkCmdDrawIndexed(cmdBuff, GetIndexBuffer().GetNumIndices(), 1, 0, 0, 0);
	}

	void Drawable::Update()
	{
		UniformBufferObject ubo{};
		// ubo.model = glm::scale(glm::mat4(), m_Scale) * glm::rotate(glm::mat4(), 0.0f, m_Rotation)
		// * 			glm::translate(glm::mat4(), m_Translation);

		// VK_TODO: Rotation not working
		ubo.model = glm::scale(glm::mat4(1.0f), m_Scale) *
					glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
					glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
					glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
					glm::translate(glm::mat4(1.0f), m_Translation);

		ubo.view = Graphics::Get().SceneCamera.Matrices.View;
		ubo.proj = Graphics::Get().SceneCamera.Matrices.Perspective;

		m_UniformHandler.Push("model", ubo.model);
		m_UniformHandler.Push("proj", ubo.proj);
		m_UniformHandler.Push("view", ubo.view);
	}

	void Drawable::EmplaceBindable(Ref<Bindable> bindable)
	{
		if (dynamic_cast<IndexBuffer*>(bindable.get()))
			m_IndexBuffer = (IndexBuffer*)bindable.get();
		else if (dynamic_cast<GraphicsPipeline*>(bindable.get()))
		{
			m_GraphicsPipeline = (GraphicsPipeline*)bindable.get();
			m_DescriptorsHandler = DescriptorsHandler(GetGraphicsPipeline());
		}
		// else if (dynamic_cast<UniformBuffer*>(bindable.get()))
		//	m_UniformBuffer = (UniformBuffer*)bindable.get();

		m_Bindables.emplace_back(std::move(bindable));
	}
}  // namespace At0::VulkanTesting

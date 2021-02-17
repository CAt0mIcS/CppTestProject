#include "Drawable.h"
#include "Graphics/Graphics.h"

#include "Graphics/Vulkan/Commands/CommandBuffer.h"
#include "Graphics/Vulkan/Bindable.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"
#include "Graphics/Vulkan/IndexBuffer.h"


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
		glm::mat4 model = glm::scale(glm::mat4(1.0f), m_Scale) *
						  glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
						  glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						  glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						  glm::translate(glm::mat4(1.0f), m_Translation);

		glm::mat4 view = Graphics::Get().SceneCamera.Matrices.View;
		glm::mat4 proj = Graphics::Get().SceneCamera.Matrices.Perspective;

		m_UniformHandler.Push("model", model);
		m_UniformHandler.Push("proj", proj);
		m_UniformHandler.Push("view", view);
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

		m_Bindables.emplace_back(std::move(bindable));
	}
}  // namespace At0::VulkanTesting

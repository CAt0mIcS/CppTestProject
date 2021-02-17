#include "pch.h"
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
		TransformComponent& tform = m_Entity.Get<TransformComponent>();

		glm::mat4 model = tform.GetMatrix();
		glm::mat4 view = Graphics::Get().SceneCamera.Matrices.View;
		glm::mat4 proj = Graphics::Get().SceneCamera.Matrices.Perspective;

		// Log::Debug("Model: {0}", glm::to_string(model));
		// Log::Debug("View: {0}", glm::to_string(view));
		// Log::Debug("Proj: {0}", glm::to_string(proj));

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

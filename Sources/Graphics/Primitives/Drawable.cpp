#include "pch.h"
#include "Drawable.h"
#include "Graphics/Graphics.h"

#include "Graphics/Vulkan/Commands/CommandBuffer.h"
#include "Graphics/Vulkan/Bindable.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"
#include "Graphics/Vulkan/IndexBuffer.h"


namespace At0::VulkanTesting
{
	void Drawable::CmdBind(const CommandBuffer& cmdBuff)
	{
		mvpdescriptorSet->Bind(cmdBuff, GetGraphicsPipeline());
		for (Ref<Bindable>& bindable : m_Bindables)
		{
			bindable->Bind(cmdBuff);
		}
	}

	void Drawable::CmdDraw(const CommandBuffer& cmdBuff)
	{
		vkCmdDrawIndexed(cmdBuff, GetIndexBuffer().GetNumIndices(), 1, 0, 0, 0);
	}

	void Drawable::Update()
	{
		TransformComponent& tform = m_Entity.Get<TransformComponent>();

		glm::mat4 modelView = Graphics::Get().SceneCamera.Matrices.View * tform.GetMatrix();
		// mvUniformBuffer->Update(&modelView);

		glm::mat4 mvp = Graphics::Get().SceneCamera.Matrices.Perspective * modelView;
		mvpUniformBuffer->Update(&mvp);
	}

	Drawable::~Drawable() {}

	void Drawable::EmplaceBindable(Ref<Bindable> bindable)
	{
		if (dynamic_cast<IndexBuffer*>(bindable.get()))
			m_IndexBuffer = (IndexBuffer*)bindable.get();
		else if (dynamic_cast<GraphicsPipeline*>(bindable.get()))
		{
			m_GraphicsPipeline = (GraphicsPipeline*)bindable.get();
			// m_DescriptorsHandler = DescriptorsHandler(GetGraphicsPipeline());

			// mvdescriptorSet = MakeScope<DescriptorSet>(GetGraphicsPipeline());
			mvpdescriptorSet = MakeScope<DescriptorSet>(GetGraphicsPipeline());

			const Shader& shader = GetGraphicsPipeline().GetShader();
			// mvUniformBuffer = MakeScope<UniformBuffer>(
			//	shader.GetUniformBlock("Transforms")->GetUniform("modelView")->GetSize());
			mvpUniformBuffer = MakeScope<UniformBuffer>(
				shader.GetUniformBlock("Transforms")->GetUniform("modelViewProj")->GetSize());

			// mvdescriptorSet->Update(*mvUniformBuffer);
			mvpdescriptorSet->Update(*mvpUniformBuffer);
		}

		m_Bindables.emplace_back(std::move(bindable));
	}
}  // namespace At0::VulkanTesting

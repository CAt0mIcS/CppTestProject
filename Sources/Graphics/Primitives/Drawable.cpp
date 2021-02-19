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
		// m_UniformHandler->BindDescriptors(cmdBuff, GetGraphicsPipeline());
		descriptorSet->Bind(cmdBuff, GetGraphicsPipeline());
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
		TransformComponent& tform = GetEntity().Get<TransformComponent>();
		glm::mat4 modelView = Graphics::Get().SceneCamera.Matrices.View * tform.GetMatrix();
		glm::mat4 modelViewProj = Graphics::Get().SceneCamera.Matrices.Perspective * modelView;


		void* data;
		uniformBuffer->MapMemory(&data);

		memcpy((char*)data + GetGraphicsPipeline()
								 .GetShader()
								 .GetUniformBlock("Transforms")
								 ->GetUniform("modelView")
								 ->GetOffset(),
			&modelView, sizeof(modelView));
		memcpy((char*)data + GetGraphicsPipeline()
								 .GetShader()
								 .GetUniformBlock("Transforms")
								 ->GetUniform("modelViewProj")
								 ->GetOffset(),
			&modelViewProj, sizeof(modelViewProj));
		uniformBuffer->UnmapMemory();

		//(*m_UniformHandler)["modelView"] = modelView;
		//(*m_UniformHandler)["modelViewProj"] = modelViewProj;
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

			// m_UniformHandler = MakeScope<UniformHandler>(GetGraphicsPipeline());

			uniformBuffer = MakeScope<UniformBuffer>(sizeof(glm::mat4) * 2);
			descriptorSet = MakeScope<DescriptorSet>(GetGraphicsPipeline());

			auto writeDesc =
				uniformBuffer->GetWriteDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			writeDesc.GetWriteDescriptorSet().dstSet = *descriptorSet;

			descriptorSet->Update({ writeDesc });
		}

		m_Bindables.emplace_back(std::move(bindable));
	}
}  // namespace At0::VulkanTesting

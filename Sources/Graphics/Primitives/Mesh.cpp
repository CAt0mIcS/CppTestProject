#include "pch.h"
#include "Mesh.h"

#include "Graphics/Vulkan/GraphicsPipeline.h"


namespace At0::VulkanTesting
{
	Mesh::Mesh(std::vector<Ref<Bindable>> bindables)
	{
		for (auto& bindable : bindables)
		{
			EmplaceBindable(bindable);
		}
	}

	void Mesh::CmdDraw(const CommandBuffer& cmdBuff)
	{
		m_DescriptorsHandler.Push("UniformBufferObject", m_UniformHandler);
		m_DescriptorsHandler.Update(GetGraphicsPipeline());
		m_DescriptorsHandler.BindDescriptor(cmdBuff, GetGraphicsPipeline());

		Drawable::CmdDraw(cmdBuff);
	}
}  // namespace At0::VulkanTesting

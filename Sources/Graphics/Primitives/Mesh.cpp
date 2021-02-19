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

	void Mesh::CmdBind(const CommandBuffer& cmdBuff)
	{
		m_DescriptorsHandler.Push("UniformBufferObject", m_UniformHandler);
		Drawable::CmdBind(cmdBuff);
	}
}  // namespace At0::VulkanTesting

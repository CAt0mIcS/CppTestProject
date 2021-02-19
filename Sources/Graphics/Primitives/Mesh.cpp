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

	void Mesh::CmdDraw(const CommandBuffer& cmdBuff) { Drawable::CmdDraw(cmdBuff); }
}  // namespace At0::VulkanTesting

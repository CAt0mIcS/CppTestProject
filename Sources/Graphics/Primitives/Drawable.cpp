#include "Drawable.h"

#include "Graphics/Vulkan/Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	void Drawable::Draw(CommandBuffer& cmdBuff)
	{
		for (std::shared_ptr<Bindable>& bindable : m_Bindables)
		{
			bindable->Bind(cmdBuff);
		}

		vkCmdDrawIndexed(cmdBuff, GetIndexBuffer().GetNumIndices(), 1, 0, 0, 0);
	}

	void Drawable::EmplaceBindable(std::shared_ptr<Bindable> bindable)
	{
		if (dynamic_cast<IndexBuffer*>(bindable.get()))
			m_IndexBuffer = (IndexBuffer*)bindable.get();
		else if (dynamic_cast<GraphicsPipeline*>(bindable.get()))
			m_GraphicsPipeline = (GraphicsPipeline*)bindable.get();

		m_Bindables.emplace_back(std::move(bindable));
	}
}  // namespace At0::VulkanTesting

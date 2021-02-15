#include "Drawable.h"

#include "Graphics/Vulkan/Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	void Drawable::Draw(CommandBuffer& cmdBuff)
	{
		for (Codex::SharedPointer<Bindable>& bindable : m_Bindables)
		{
			bindable->Bind(cmdBuff);
		}

		vkCmdDrawIndexed(cmdBuff, GetNumberOfIndices(), 1, 0, 0, 0);
	}

	Drawable::~Drawable()
	{
		// for (Codex::SharedPointer<Bindable>& bindable : m_Bindables)
		//{
		//	long refCoung = bindable.use_count();
		//	bindable.Reset();
		//	refCoung = bindable.use_count();
		//	int i = 0;
		//}
	}

	void Drawable::EmplaceBindable(std::shared_ptr<Bindable> bindable)
	{
		if (dynamic_cast<IndexBuffer*>(bindable.get()))
			m_NumIndices = ((IndexBuffer*)bindable.get())->GetNumIndices();

		m_Bindables.emplace_back(std::move(bindable));
	}
}  // namespace At0::VulkanTesting

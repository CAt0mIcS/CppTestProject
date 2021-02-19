#include "pch.h"
#include "DescriptorsHandler.h"

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/LogicalDevice.h"
#include "Graphics/Vulkan/Pipeline.h"
#include "UniformHandler.h"

#include "Graphics/Graphics.h"
#include "Utils/RAssert.h"


namespace At0::VulkanTesting
{
	DescriptorsHandler::DescriptorsHandler(const Pipeline& pipeline)
		: m_Shader(&pipeline.GetShader()), m_PushDescriptors(false),
		  m_DescriptorSet(MakeScope<DescriptorSet>(pipeline)), m_Changed(true)
	{
	}

	void DescriptorsHandler::Push(const std::string& descriptorName, UniformHandler& uniformHandler)
	{
		RAY_MEXPECTS(m_Shader, "DescriptorsHandler was not created with pipeline");
		uniformHandler.Update(m_Shader->GetUniformBlock(descriptorName));
		Push(descriptorName, uniformHandler.GetUniformBuffer());
	}

	bool DescriptorsHandler::Update(const Pipeline& pipeline)
	{
		if (m_Shader != &pipeline.GetShader())
		{
			m_Shader = &pipeline.GetShader();
			m_PushDescriptors = false;
			m_Descriptors.clear();
			m_WriteDescriptorSets.clear();

			if (!m_PushDescriptors)
			{
				m_DescriptorSet = MakeScope<DescriptorSet>(pipeline);
			}

			m_Changed = false;
			return false;
		}

		if (m_Changed)
		{
			m_WriteDescriptorSets.clear();
			m_WriteDescriptorSets.reserve(m_Descriptors.size());

			for (const auto& [descriptorName, descriptor] : m_Descriptors)
			{
				auto writeDescriptorSet = descriptor.writeDescriptor.GetWriteDescriptorSet();
				writeDescriptorSet.dstSet = VK_NULL_HANDLE;

				if (!m_PushDescriptors)
					writeDescriptorSet.dstSet = *m_DescriptorSet;

				m_WriteDescriptorSets.emplace_back(writeDescriptorSet);
			}

			if (!m_PushDescriptors)
				m_DescriptorSet->Update(m_WriteDescriptorSets);

			m_Changed = false;
		}

		return true;
	}

	void DescriptorsHandler::BindDescriptor(
		const CommandBuffer& commandBuffer, const Pipeline& pipeline)
	{
		if (m_PushDescriptors)
		{
			VulkanInstance::FvkCmdPushDescriptorSetKHR(Graphics::Get().GetLogicalDevice(),
				commandBuffer, pipeline.GetBindPoint(), pipeline.GetLayout(), 0,
				static_cast<uint32_t>(m_WriteDescriptorSets.size()), m_WriteDescriptorSets.data());
		}
		else
		{
			m_DescriptorSet->Bind(commandBuffer);
		}
	}
}  // namespace At0::VulkanTesting

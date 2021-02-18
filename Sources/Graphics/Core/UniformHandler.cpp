#include "pch.h"
#include "UniformHandler.h"

#include "Graphics/Vulkan/Descriptor.h"
#include "Graphics/Vulkan/UniformBuffer.h"
#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Shader.h"


namespace At0::VulkanTesting
{
	UniformHandler::UniformHandler(const Pipeline& pipeline)
	{
		const Shader& shader = pipeline.GetShader();
		m_DescriptorSets.reserve(shader.GetUniformBlocks().size());
		m_UniformBuffers.reserve(m_DescriptorSets.size());

		for (const auto& uniformBlock : shader.GetUniformBlocks())
		{
			for (const auto& uniform : uniformBlock.second.GetUniforms())
			{
				m_DescriptorSets.emplace_back(new DescriptorSet(pipeline));
				m_UniformBuffers.emplace(
					uniform.first, new UniformBuffer(uniform.second.GetSize()));
			}
		}

		UpdateDescriptors();
	}

	UniformHandler::~UniformHandler()
	{
		for (auto& it : m_UniformBuffers)
			if (it.second)
				delete it.second;

		for (DescriptorSet* descSet : m_DescriptorSets)
			if (descSet)
				delete descSet;
	}

	void UniformHandler::BindDescriptors(const CommandBuffer& cmdBuff, const Pipeline& pipeline)
	{
		for (DescriptorSet* descSet : m_DescriptorSets)
			descSet->Bind(cmdBuff, pipeline);
	}

	void UniformHandler::UpdateDescriptors()
	{
		std::vector<UniformBuffer*> uniformBuffers;
		for (const auto& it : m_UniformBuffers)
		{
			uniformBuffers.emplace_back(it.second);
		}

		for (uint32_t i = 0; i < uniformBuffers.size(); ++i)
		{
			m_DescriptorSets[i]->Update(*uniformBuffers[i]);
		}
	}

	UniformBufferView UniformHandler::operator[](std::string_view uniformName)
	{
		return { *m_UniformBuffers[uniformName.data()] };
	}
}  // namespace At0::VulkanTesting
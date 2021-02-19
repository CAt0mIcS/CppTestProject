#include "pch.h"
#include "UniformHandler.h"

#include "Graphics/Vulkan/Descriptor.h"
#include "Graphics/Vulkan/UniformBuffer.h"
#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Shader.h"


namespace At0::VulkanTesting
{
	UniformHandler::UniformHandler(const Pipeline& pipeline) : m_Shader(pipeline.GetShader())
	{
		for (auto& [uniformBlockName, uniformBlock] : pipeline.GetShader().GetUniformBlocks())
		{
			UniformBuffer* buffer = new UniformBuffer(uniformBlock.GetSize());
			DescriptorSet* descSet = new DescriptorSet(pipeline);
			auto writeDesc = buffer->GetWriteDescriptor(
				uniformBlock.GetBinding(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			writeDesc.GetWriteDescriptorSet().dstSet = *descSet;
			descSet->Update({ writeDesc });

			m_Uniforms.emplace(uniformBlockName, UniformData{ buffer, descSet, {} });

			for (auto& [uniformName, uniform] : uniformBlock.GetUniforms())
			{
				m_Uniforms[uniformBlockName].names.emplace_back(uniformName);
			}
		}
	}

	UniformHandler::~UniformHandler()
	{
		for (auto& it : m_Uniforms)
		{
			delete it.second.uniformBuffer;
			delete it.second.descriptorSet;
		}
	}

	void UniformHandler::Bind(const CommandBuffer& cmdBuff, const Pipeline& pipeline)
	{
		for (auto& it : m_Uniforms)
		{
			it.second.descriptorSet->Bind(cmdBuff, pipeline);
		}
	}

	UniformBlockView UniformHandler::operator[](std::string_view uniformBlockName)
	{
		if (auto& it = m_Uniforms.find(uniformBlockName.data()); it != m_Uniforms.end())
		{
			return { &it->second, m_Shader.GetUniformBlock(uniformBlockName) };
		}

		return { nullptr, std::nullopt };
	}
}  // namespace At0::VulkanTesting

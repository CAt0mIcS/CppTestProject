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
		for (const auto& uniformBlock : shader.GetUniformBlocks())
		{
			std::unordered_map<std::string, UniformBuffer*> uniformBuffers;
			for (const auto& uniform : uniformBlock.second.GetUniforms())
			{
				uniformBuffers.emplace(uniform.first, new UniformBuffer(uniform.second.GetSize()));
			}

			m_Uniforms.emplace(
				uniformBlock.first, UniformPair{ uniformBuffers, new DescriptorSet(pipeline) });
		}

		UpdateDescriptors();
	}

	UniformHandler::~UniformHandler()
	{
		for (auto& it : m_Uniforms)
		{
			for (auto& buffer : it.second.uniformBuffers)
				delete buffer.second;

			delete it.second.descriptorSet;
		}
	}

	void UniformHandler::BindDescriptors(const CommandBuffer& cmdBuff, const Pipeline& pipeline)
	{
		for (auto& it : m_Uniforms)
			it.second.descriptorSet->Bind(cmdBuff, pipeline);
	}

	void UniformHandler::UpdateDescriptors()
	{
		for (auto& it : m_Uniforms)
		{
			std::vector<VkWriteDescriptorSet> writeDescriptors;
			for (auto& buffer : it.second.uniformBuffers)
			{
				auto writeDesc =
					buffer.second->GetWriteDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
				writeDesc.GetWriteDescriptorSet().dstSet = *it.second.descriptorSet;
				writeDescriptors.emplace_back(std::move(writeDesc));
			}

			it.second.descriptorSet->Update({ writeDescriptors });
		}
	}

	UniformBufferView UniformHandler::operator[](std::string_view uniformName)
	{
		for (auto& it : m_Uniforms)
		{
			auto& itBuff = it.second.uniformBuffers.find(uniformName.data());
			if (itBuff == it.second.uniformBuffers.end())
				return { nullptr };

			return { itBuff->second };
		}

		return { nullptr };
	}
}  // namespace At0::VulkanTesting

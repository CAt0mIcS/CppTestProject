#include "UniformBuffer.h"

#include "Graphics/Graphics.h"


namespace At0::VulkanTesting
{
	UniformBuffer::UniformBuffer(const Pipeline& pipeline, std::string_view tag)
		: m_DescriptorSet(pipeline)
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer,
			m_BufferMemory);
	}

	void UniformBuffer::Update(const UniformBufferObject& ubo)
	{
		void* data;
		vkMapMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), m_BufferMemory);
	}

	VkDescriptorSetLayoutBinding UniformBuffer::GetDescriptorSetLayout(
		uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}

	std::string UniformBuffer::GetUID(const Pipeline& pipeline, std::string_view tag)
	{
		static uint64_t nextID = 0;
		if (tag.empty())
		{
			++nextID;
			return typeid(UniformBuffer).name() + std::to_string(nextID);
		}

		return typeid(UniformBuffer).name() + std::string(tag);
	}

	void UniformBuffer::Bind(const CommandBuffer& cmdBuff) { m_DescriptorSet.Bind(cmdBuff); }
}  // namespace At0::VulkanTesting

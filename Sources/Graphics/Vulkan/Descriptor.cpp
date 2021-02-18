#include "pch.h"
#include "Descriptor.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/LogicalDevice.h"
#include "Graphics/Vulkan/UniformBuffer.h"


namespace At0::VulkanTesting
{
	DescriptorSet::DescriptorSet(const Pipeline& pipeline)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pipeline.GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &pipeline.GetDescriptorSetLayout();

		RAY_VK_THROW_FAILED(vkAllocateDescriptorSets(
								Graphics::Get().GetLogicalDevice(), &allocInfo, &m_DescriptorSet),
			"Failed to allocate descriptor set");
	}

	DescriptorSet::~DescriptorSet() {}

	void DescriptorSet::Bind(const CommandBuffer& cmdBuff, const Pipeline& pipeline)
	{
		vkCmdBindDescriptorSets(cmdBuff, pipeline.GetBindPoint(), pipeline.GetLayout(), 0, 1,
			&m_DescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Update(const UniformBuffer& uniformBuffer)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = uniformBuffer.GetSize();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSet;
		descriptorWrite.dstBinding = 0;	 // VK_TODO: Get binding from shader
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(Graphics::Get().GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}  // namespace At0::VulkanTesting
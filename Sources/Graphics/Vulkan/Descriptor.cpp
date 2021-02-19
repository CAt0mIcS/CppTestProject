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

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites)
	{
		vkUpdateDescriptorSets(Graphics::Get().GetLogicalDevice(),
			(uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}  // namespace At0::VulkanTesting

#include "pch.h"
#include "Descriptor.h"
#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/LogicalDevice.h"


namespace At0::VulkanTesting
{
	DescriptorSet::DescriptorSet(const Pipeline& pipeline)
		: m_PipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
		  m_PipelineLayout(pipeline.GetLayout()),
		  m_PipelineDescriptorPool(pipeline.GetDescriptorPool())
	{
		VkDescriptorSetLayout layout = pipeline.GetDescriptorSetLayout();

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pipeline.GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		RAY_VK_THROW_FAILED(vkAllocateDescriptorSets(
								Graphics::Get().GetLogicalDevice(), &allocInfo, &m_DescriptorSet),
			"Failed to allocate descriptor set");
	}

	DescriptorSet::~DescriptorSet()
	{
		// vkFreeDescriptorSets(
		//	Graphics::Get().GetLogicalDevice(), m_PipelineDescriptorPool, 1, &m_DescriptorSet);
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites)
	{
		vkUpdateDescriptorSets(Graphics::Get().GetLogicalDevice(),
			(uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::Bind(const CommandBuffer& cmdBuff)
	{
		vkCmdBindDescriptorSets(
			cmdBuff, m_PipelineBindPoint, m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
	}
}  // namespace At0::VulkanTesting

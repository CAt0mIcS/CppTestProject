#include "Descriptor.h"
#include "Graphics/Graphics.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	DescriptorSet::DescriptorSet(const GraphicsPipeline& pipeline)
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
		// vkFreeDescriptorSets(Graphics::Get().GetLogicalDevice(),
		//	Graphics::Get().GetDescriptorPool(), 1, &m_DescriptorSet);
	}
}  // namespace At0::VulkanTesting

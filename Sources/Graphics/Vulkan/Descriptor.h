#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>


namespace At0::VulkanTesting
{
	class DescriptorSetLayout;
	class GraphicsPipeline;

	class DescriptorSet
	{
	public:
		DescriptorSet(const GraphicsPipeline& pipeline);
		~DescriptorSet();

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

	private:
		VkDescriptorSet m_DescriptorSet;
	};
}  // namespace At0::VulkanTesting

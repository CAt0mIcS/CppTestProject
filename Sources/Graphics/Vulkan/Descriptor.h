#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>


namespace At0::VulkanTesting
{
	class DescriptorSetLayout;
	class Pipeline;
	class CommandBuffer;

	class DescriptorSet
	{
	public:
		DescriptorSet(const Pipeline& pipeline);
		~DescriptorSet();

		static void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);

		void Bind(const CommandBuffer& cmdBuff);

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

	private:
		VkDescriptorSet m_DescriptorSet;

		VkPipelineBindPoint m_PipelineBindPoint;
		VkPipelineLayout m_PipelineLayout;
		VkDescriptorPool m_PipelineDescriptorPool;
	};
}  // namespace At0::VulkanTesting

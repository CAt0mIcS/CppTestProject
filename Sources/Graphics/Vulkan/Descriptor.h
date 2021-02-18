#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Pipeline;
	class CommandBuffer;
	class UniformBuffer;

	class DescriptorSet
	{
	public:
		DescriptorSet(const Pipeline& pipeline);
		~DescriptorSet();

		void Bind(const CommandBuffer& cmdBuff, const Pipeline& pipeline);
		void Update(const UniformBuffer& uniformBuffer);

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

	private:
		VkDescriptorSet m_DescriptorSet;
	};
}  // namespace At0::VulkanTesting

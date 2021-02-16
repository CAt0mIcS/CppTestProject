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


	class DescriptorPool
	{
	public:
		DescriptorPool(const std::vector<VkDescriptorPoolSize>& size, uint32_t maxSets);
		~DescriptorPool();

		operator const VkDescriptorPool&() const { return m_DescriptorPool; }

	private:
		VkDescriptorPool m_DescriptorPool;
	};


	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout(
			uint32_t binding, VkDescriptorType descType, VkShaderStageFlags shaderStage);
		~DescriptorSetLayout();

		uint32_t GetCount() const { return m_Count; }

		operator const VkDescriptorSetLayout&() const { return m_Layout; }

	private:
		VkDescriptorSetLayout m_Layout;
		uint32_t m_Count;
	};
}  // namespace At0::VulkanTesting

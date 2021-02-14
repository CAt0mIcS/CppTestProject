#include "Descriptor.h"
#include "Graphics/Graphics.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	DescriptorSet::DescriptorSet(const DescriptorSetLayout& layout)
	{
		VkDescriptorSetLayout actualLayout = layout;

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = Graphics::Get().GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &actualLayout;

		RAY_VK_THROW_FAILED(vkAllocateDescriptorSets(
								Graphics::Get().GetLogicalDevice(), &allocInfo, &m_DescriptorSet),
			"Failed to allocate descriptor set");
	}

	DescriptorSet::~DescriptorSet()
	{
		// vkFreeDescriptorSets(Graphics::Get().GetLogicalDevice(),
		//	Graphics::Get().GetDescriptorPool(), 1, &m_DescriptorSet);
	}

	DescriptorPool::DescriptorPool(const std::vector<VkDescriptorPoolSize>& sizes, uint32_t maxSets)
	{
		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.maxSets = maxSets;
		createInfo.poolSizeCount = sizes.size();
		createInfo.pPoolSizes = sizes.data();

		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_DescriptorPool),
			"Failed to create descriptor pool.");
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(Graphics::Get().GetLogicalDevice(), m_DescriptorPool, nullptr);
	}

	DescriptorSetLayout::DescriptorSetLayout(
		uint32_t binding, VkDescriptorType descType, VkShaderStageFlags shaderStage)
	{
		VkDescriptorSetLayoutBinding descBinding{};
		descBinding.binding = binding;
		descBinding.descriptorType = descType;
		descBinding.descriptorCount = 1;
		descBinding.stageFlags = shaderStage;
		descBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &descBinding;

		RAY_VK_THROW_FAILED(vkCreateDescriptorSetLayout(Graphics::Get().GetLogicalDevice(),
								&layoutInfo, nullptr, &m_Layout),
			"Failed to create descriptor set layout.");
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(Graphics::Get().GetLogicalDevice(), m_Layout, nullptr);
	}
}  // namespace At0::VulkanTesting
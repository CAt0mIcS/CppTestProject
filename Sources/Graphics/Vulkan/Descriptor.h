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


	class WriteDescriptorSet
	{
	public:
		WriteDescriptorSet(
			const VkWriteDescriptorSet& writeDescriptorSet, const VkDescriptorImageInfo& imageInfo)
			: m_WriteDescriptorSet(writeDescriptorSet),
			  m_ImageInfo(std::make_unique<VkDescriptorImageInfo>(imageInfo))
		{
			this->m_WriteDescriptorSet.pImageInfo = this->m_ImageInfo.get();
		}

		WriteDescriptorSet(const VkWriteDescriptorSet& writeDescriptorSet,
			const VkDescriptorBufferInfo& bufferInfo)
			: m_WriteDescriptorSet(writeDescriptorSet),
			  m_BufferInfo(std::make_unique<VkDescriptorBufferInfo>(bufferInfo))
		{
			this->m_WriteDescriptorSet.pBufferInfo = this->m_BufferInfo.get();
		}

		const VkWriteDescriptorSet& GetWriteDescriptorSet() const { return m_WriteDescriptorSet; }

	private:
		VkWriteDescriptorSet m_WriteDescriptorSet;
		std::unique_ptr<VkDescriptorImageInfo> m_ImageInfo;
		std::unique_ptr<VkDescriptorBufferInfo> m_BufferInfo;
	};

	class Descriptor
	{
	public:
		Descriptor() = default;

		virtual ~Descriptor() = default;

		virtual WriteDescriptorSet GetWriteDescriptor(
			uint32_t binding, VkDescriptorType descriptorType) const = 0;
	};

}  // namespace At0::VulkanTesting

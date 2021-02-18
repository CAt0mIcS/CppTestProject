#pragma once

#include <string>

#include "../Descriptor.h"


namespace At0::VulkanTesting
{
	class Sampler;
	class ImageView;

	class Image : public Descriptor
	{
	public:
		Image(const std::string_view filepath);
		virtual ~Image();

		static void CreateImage(uint32_t width, uint32_t height, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
			VkImage& image, VkDeviceMemory& imageMemory);
		static void TransitionImageLayout(
			VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void CopyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(
			uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlag, uint32_t count);
		WriteDescriptorSet GetWriteDescriptor(
			uint32_t binding, VkDescriptorType descriptorType) const override;

	private:
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;

		Scope<Sampler> m_Sampler;
		Scope<ImageView> m_ImageView;
		VkImageLayout m_Layout;
	};
}  // namespace At0::VulkanTesting

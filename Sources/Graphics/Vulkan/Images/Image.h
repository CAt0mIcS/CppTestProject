#pragma once

#include <string>
#include <vector>

#include "Base.h"

#include "../Descriptor.h"
#include "ImageView.h"
#include "../Textures/Sampler.h"


namespace At0::VulkanTesting
{
	class Sampler;

	class Image : public Descriptor
	{
	public:
		Image(const std::string_view filepath);
		Image() = default;
		virtual ~Image();

		operator const VkImage&() const { return m_Image; }
		const VkImageView& GetImageView() const { return *m_ImageView; }

		static void CreateImage(uint32_t width, uint32_t height, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
			VkImage& image, VkDeviceMemory& imageMemory);
		static void TransitionImageLayout(
			VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void CopyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(
			uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlag, uint32_t count);
		virtual WriteDescriptorSet GetWriteDescriptor(
			uint32_t binding, VkDescriptorType descriptorType) const override;
		static std::vector<VkFormat> FindSupportedFormats(
			std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	protected:
		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;

		Scope<Sampler> m_Sampler = nullptr;
		Scope<ImageView> m_ImageView = nullptr;
		VkImageLayout m_Layout = VK_IMAGE_LAYOUT_UNDEFINED;
	};
}  // namespace At0::VulkanTesting

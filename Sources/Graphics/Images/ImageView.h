#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class ImageView
	{
	public:
		ImageView() = default;
		ImageView(VkImage image, VkFormat imageFormat);
		~ImageView();

	private:
		VkImageView m_View = nullptr;
	};
}  // namespace At0::VulkanTesting

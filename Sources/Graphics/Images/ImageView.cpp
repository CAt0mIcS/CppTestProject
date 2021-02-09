#include "ImageView.h"
#include "Graphics/Graphics.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	ImageView::ImageView(VkImage image, VkFormat imageFormat)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = imageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		RAY_VK_THROW_FAILED(
			vkCreateImageView(Graphics::Get().GetLogicalDevice(), &createInfo, nullptr, &m_View),
			"Failed to create image view.");
	}

	ImageView::~ImageView()
	{
		// VT_TODO: Destroy image
		// if (m_View)
		// vkDestroyImageView(Graphics::Get().GetLogicalDevice(), m_View, nullptr);
	}
}  // namespace At0::VulkanTesting

#include "pch.h"
#include "DepthImage.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/Swapchain.h"
#include "../Textures/Sampler.h"

namespace At0::VulkanTesting
{
	DepthImage::DepthImage()
	{
		m_DepthFormat = FindSupportedFormats(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)[0];

		CreateImage(Graphics::Get().GetSwapchain().GetExtent().width,
			Graphics::Get().GetSwapchain().GetExtent().height, m_DepthFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);

		m_ImageView = MakeScope<ImageView>(m_Image, m_DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	bool DepthImage::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}  // namespace At0::VulkanTesting

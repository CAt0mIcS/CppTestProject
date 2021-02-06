#include "Swapchain.h"
#include "Graphics.h"
#include "Window.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Swapchain::Swapchain()
	{
		SupportDetails supportDetails = QuerySwapchainSupport();
		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetails.Formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(supportDetails.PresentModes);
		m_Extent = ChooseExtent(supportDetails.Capabilities);

		uint32_t imageCount = supportDetails.Capabilities.minImageCount + 1;
		// Make sure we don't exceed the max image count
		if (supportDetails.Capabilities.maxImageCount > 0 &&
			imageCount > supportDetails.Capabilities.maxImageCount)
		{
			imageCount = supportDetails.Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = Graphics::Get().GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Queue families
		uint32_t queueFamilyIndices[] = { Graphics::Get().GetLogicalDevice().GetGraphicsFamily(),
			Graphics::Get().GetLogicalDevice().GetPresentFamily() };

		if (Graphics::Get().GetLogicalDevice().GetGraphicsFamily() !=
			Graphics::Get().GetLogicalDevice().GetPresentFamily())
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = supportDetails.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		RAY_VK_THROW_FAILED(vkCreateSwapchainKHR(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_Swapchain),
			"Failed to create the swapchain.");
	}

	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(Graphics::Get().GetLogicalDevice(), m_Swapchain, nullptr);
	}

	Swapchain::SupportDetails Swapchain::QuerySwapchainSupport()
	{
		SupportDetails supportDetails;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &supportDetails.Capabilities);

		uint32_t surfaceFormats;
		vkGetPhysicalDeviceSurfaceFormatsKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &surfaceFormats, nullptr);

		supportDetails.Formats.resize(surfaceFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &surfaceFormats, supportDetails.Formats.data());

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &presentModeCount, nullptr);

		supportDetails.PresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(Graphics::Get().GetPhysicalDevice(),
			Graphics::Get().GetSurface(), &presentModeCount, supportDetails.PresentModes.data());

		return supportDetails;
	}

	VkSurfaceFormatKHR Swapchain::ChooseSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& formats) const
	{
		for (const auto& format : formats)
		{
			if (format.format == VK_FORMAT_R8G8B8A8_SRGB &&
				format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;
		}

		return formats.front();
	}

	VkPresentModeKHR Swapchain::ChoosePresentMode(
		const std::vector<VkPresentModeKHR>& presentModes) const
	{
		for (const VkPresentModeKHR& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D Swapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		else
		{
			int width, height;
			Window::Get().GetSize(&width, &height);

			VkExtent2D extent = { (uint32_t)width, (uint32_t)height };
			extent.width = std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, extent.width));
			extent.height = std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, extent.height));
			return extent;
		}
	}

}  // namespace At0::VulkanTesting

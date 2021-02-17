#include "pch.h"
#include "Swapchain.h"

#include "Graphics/Graphics.h"
#include "Surface.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Window.h"

#include <algorithm>
#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Swapchain::Swapchain(Swapchain* previousSwapchain)
	{
		SupportDetails supportDetails = QuerySwapchainSupport();
		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetails.Formats);
		m_Format = surfaceFormat.format;
		VkPresentModeKHR presentMode = ChoosePresentMode(supportDetails.PresentModes);
		m_Extent = ChooseExtent(supportDetails.Capabilities);

		uint32_t imageCount = supportDetails.Capabilities.minImageCount + 1;
		// Make sure we don't exceed the max image count. 0 means that there is no maximum limit
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
		if (previousSwapchain)
			createInfo.oldSwapchain = *previousSwapchain;
		else
			createInfo.oldSwapchain = VK_NULL_HANDLE;

		RAY_VK_THROW_FAILED(vkCreateSwapchainKHR(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_Swapchain),
			"Failed to create the swapchain.");

		// Retrieve swapchain images
		uint32_t swapchainImages;
		vkGetSwapchainImagesKHR(
			Graphics::Get().GetLogicalDevice(), m_Swapchain, &swapchainImages, nullptr);

		m_Images.resize(swapchainImages);
		vkGetSwapchainImagesKHR(
			Graphics::Get().GetLogicalDevice(), m_Swapchain, &swapchainImages, m_Images.data());

		// Create image views
		m_ImageViews.resize(m_Images.size());
		for (uint32_t i = 0; i < m_ImageViews.size(); ++i)
		{
			m_ImageViews[i] = MakeScope<ImageView>(m_Images[i], m_Format);
		}
	}

	Swapchain::~Swapchain()
	{
		// Destroy image views before swapchain (not neccessary I think)
		m_ImageViews.clear();
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
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
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
			Window::Get().GetFramebufferSize(&width, &height);

			VkExtent2D extent = { (uint32_t)width, (uint32_t)height };

			// Clamp values to allowed minimum and maximum implementation extents supported
			extent.width = std::clamp(
				extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(extent.height, capabilities.minImageExtent.height,
				capabilities.maxImageExtent.height);

			return extent;
		}
	}

}  // namespace At0::VulkanTesting

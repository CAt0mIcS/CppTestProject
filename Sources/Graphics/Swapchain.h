#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

#include "Images/ImageView.h"


namespace At0::VulkanTesting
{
	class Swapchain
	{
	public:
		struct SupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

	public:
		Swapchain();
		~Swapchain();

		SupportDetails QuerySwapchainSupport();
		VkFormat GetFormat() const { return m_Format; }

	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& formats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	private:
		VkSwapchainKHR m_Swapchain;
		VkFormat m_Format;
		VkExtent2D m_Extent;

		std::vector<VkImage> m_Images;
		std::vector<ImageView> m_ImageViews;
	};
}  // namespace At0::VulkanTesting

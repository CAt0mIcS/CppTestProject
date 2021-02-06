#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>


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

	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& formats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	private:
		VkSwapchainKHR m_Swapchain;
		VkExtent2D m_Extent;
	};
}  // namespace At0::VulkanTesting

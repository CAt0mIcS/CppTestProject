#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

namespace At0::VulkanTesting
{
	class VulkanInstance;
	class PhysicalDevice;

	class Surface
	{
	public:
		Surface(const VulkanInstance* instance, const PhysicalDevice* device);
		~Surface();

		operator const VkSurfaceKHR&() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
		VkSurfaceFormatKHR m_Format;
		VkSurfaceCapabilitiesKHR m_Capabilities;

		const VulkanInstance& m_Instance;
		const PhysicalDevice& m_PhysicalDevice;
	};
}  // namespace At0::VulkanTesting

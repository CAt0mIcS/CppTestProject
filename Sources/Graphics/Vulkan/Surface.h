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
		Surface();
		~Surface();

		operator const VkSurfaceKHR&() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
	};
}  // namespace At0::VulkanTesting

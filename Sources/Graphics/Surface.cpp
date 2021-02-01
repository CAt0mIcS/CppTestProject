#include "Surface.h"
#include "../Window.h"
#include "PhysicalDevice.h"
#include "VulkanInstance.h"
#include "../Utils/RLogger.h"
#include "../Utils/RException.h"
#include "../Utils/RAssert.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Surface::Surface(const VulkanInstance* instance, const PhysicalDevice* device)
		: m_Instance(*instance), m_PhysicalDevice(*device)
	{
		Window::Get().CreateSurface(m_Instance, nullptr, &m_Surface);

		RAY_VK_THROW_FAILED(
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &m_Capabilities),
			"[Surface] Failed to get surface capabilities.");

		uint32_t surfaceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			m_PhysicalDevice, m_Surface, &surfaceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			m_PhysicalDevice, m_Surface, &surfaceFormatCount, surfaceFormats.data());

		if (surfaceFormatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			m_Format.format = VK_FORMAT_B8G8R8A8_UNORM;
			m_Format.colorSpace = surfaceFormats[0].colorSpace;
		}
		else
		{
			// Iterate over the list of available surface format and
			// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
			bool found_B8G8R8A8_UNOFM = false;

			for (auto& surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
				{
					m_Format.format = surfaceFormat.format;
					m_Format.colorSpace = surfaceFormat.colorSpace;
					found_B8G8R8A8_UNOFM = true;
					break;
				}
			}

			// In case VK_FORMAT_B8G8R8A8_UNORM is not available
			// select the first available color format
			if (!found_B8G8R8A8_UNOFM)
			{
				m_Format.format = surfaceFormats[0].format;
				m_Format.colorSpace = surfaceFormats[0].colorSpace;
			}
		}
	}

	Surface::~Surface() { vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr); }
}  // namespace At0::VulkanTesting

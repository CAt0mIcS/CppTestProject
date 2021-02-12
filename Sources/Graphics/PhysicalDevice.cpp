#include "PhysicalDevice.h"
#include "Graphics.h"
#include "../Utils/RLogger.h"
#include "../Utils/RException.h"
#include "../Utils/RAssert.h"

#include <vulkan/vulkan.h>
#include <map>


namespace At0::VulkanTesting
{
	PhysicalDevice::PhysicalDevice()
	{
		m_Device = SelectPhysicalDevice(FindPhysicalDevices());
		if (!m_Device)
			RAY_THROW_RUNTIME("Failed to find suitable GPU");

		vkGetPhysicalDeviceFeatures(m_Device, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_Device, &m_MemoryProperties);
	}

	PhysicalDevice::~PhysicalDevice() {}

	std::vector<VkPhysicalDevice> PhysicalDevice::FindPhysicalDevices() const
	{
		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(Graphics::Get().GetInstance(), &physicalDeviceCount, nullptr);

		RAY_MEXPECTS(physicalDeviceCount != 0, "Failed to find suitable GPU");

		std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(
			Graphics::Get().GetInstance(), &physicalDeviceCount, devices.data());

		return devices;
	}

	VkPhysicalDevice PhysicalDevice::SelectPhysicalDevice(
		const std::vector<VkPhysicalDevice>& devices) const
	{
		std::multimap<uint32_t, VkPhysicalDevice> scoreMap;

		for (const VkPhysicalDevice& device : devices)
		{
			// Check if requrested extensions are supported.
			uint32_t extensionPropCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropCount, nullptr);
			std::vector<VkExtensionProperties> extensionProperties(extensionPropCount);
			vkEnumerateDeviceExtensionProperties(
				device, nullptr, &extensionPropCount, extensionProperties.data());

			for (const char* extName : LogicalDevice::DeviceExtensions())
			{
				bool extFound = false;

				// Checks if the extension is in the available extensions.
				for (const auto& extension : extensionProperties)
				{
					if (strcmp(extName, extension.extensionName) == 0)
					{
						extFound = true;
						break;
					}
				}

				// Returns a score of 0 if this device is missing a required extension.
				if (!extFound)
					return 0;
			}

			uint32_t score = 0;
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(device, &props);

			// Higher score for externally added GPU
			if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1000;

			score += props.limits.maxImageDimension2D;

			scoreMap.emplace(score, device);
		}

		if (scoreMap.rbegin()->first > 0)
			return scoreMap.rbegin()->second;

		return nullptr;
	}
}  // namespace At0::VulkanTesting

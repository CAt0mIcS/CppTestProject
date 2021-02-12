﻿#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::VulkanTesting
{
	class VulkanInstance;

	class PhysicalDevice
	{
	public:
		PhysicalDevice();
		~PhysicalDevice();

		operator const VkPhysicalDevice&() const { return m_Device; }
		const VkPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }
		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const
		{
			return m_MemoryProperties;
		}

	private:
		std::vector<VkPhysicalDevice> FindPhysicalDevices() const;
		VkPhysicalDevice SelectPhysicalDevice(const std::vector<VkPhysicalDevice>& devices) const;

	private:
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;
		VkPhysicalDevice m_Device;
	};
}  // namespace At0::VulkanTesting

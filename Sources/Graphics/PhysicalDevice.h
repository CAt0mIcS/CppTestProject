#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::VulkanTesting
{
	class VulkanInstance;

	class PhysicalDevice
	{
	public:
		PhysicalDevice(const VulkanInstance* instance);
		~PhysicalDevice();

		operator const VkPhysicalDevice&() const { return m_Device; }
		const VkPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }

	private:
		std::vector<VkPhysicalDevice> FindPhysicalDevices() const;
		VkPhysicalDevice SelectPhysicalDevice(const std::vector<VkPhysicalDevice>& devices) const;

	private:
		const VulkanInstance& m_Instance;

		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDevice m_Device;
	};
}  // namespace At0::VulkanTesting

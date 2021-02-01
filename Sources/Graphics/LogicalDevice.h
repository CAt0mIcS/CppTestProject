﻿#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

namespace At0::VulkanTesting
{
	class PhysicalDevice;
	class Surface;
	class VulkanInstance;

	class LogicalDevice
	{
	public:
		LogicalDevice(const VulkanInstance* instance, const PhysicalDevice* physicalDevice,
			const Surface* surface);
		~LogicalDevice();

		operator const VkDevice&() const { return m_Device; }

		static const std::vector<const char*>& DeviceExtensions() { return s_DeviceExtensions; }

	private:
		void CreateQueueIndices();
		void CreateLogicalDevice();

	private:
		static const std::vector<const char*> s_DeviceExtensions;
		const PhysicalDevice& m_PhysicalDevice;
		const Surface& m_Surface;
		const VulkanInstance& m_Instance;

		VkDevice m_Device;

		uint32_t m_GraphicsFamily;
		uint32_t m_PresentFamily;
		uint32_t m_ComputeFamily;
		uint32_t m_TransferFamily;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkQueue m_ComputeQueue;
		VkQueue m_TransferQueue;

		VkQueueFlags m_SupportedQueues;
	};
}  // namespace At0::VulkanTesting

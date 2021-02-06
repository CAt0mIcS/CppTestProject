#pragma once

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
		LogicalDevice();
		~LogicalDevice();

		operator const VkDevice&() const { return m_Device; }

		static const std::vector<const char*>& DeviceExtensions() { return s_DeviceExtensions; }

		uint32_t GetGraphicsFamily() const { return m_GraphicsFamily; }
		uint32_t GetPresentFamily() const { return m_PresentFamily; }
		uint32_t GetComputeFamily() const { return m_ComputeFamily; }
		uint32_t GetTransferFamily() const { return m_TransferFamily; }

		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { return m_PresentQueue; }
		VkQueue GetComputeQueue() const { return m_ComputeQueue; }
		VkQueue GetTransferQueue() const { return m_TransferQueue; }

	private:
		void CreateQueueIndices();
		void CreateLogicalDevice();

	private:
		static const std::vector<const char*> s_DeviceExtensions;

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

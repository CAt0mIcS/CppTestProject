#include "LogicalDevice.h"
#include "Graphics/Graphics.h"
#include "Utils/RLogger.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"

#include <vulkan/vulkan.h>
#include <optional>


namespace At0::VulkanTesting
{
	const std::vector<const char*> LogicalDevice::s_DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	LogicalDevice::LogicalDevice()
	{
		CreateQueueIndices();
		CreateLogicalDevice();
	}

	LogicalDevice::~LogicalDevice()
	{
		vkDeviceWaitIdle(m_Device);
		vkDestroyDevice(m_Device, nullptr);
	}

	PFN_vkVoidFunction LogicalDevice::LoadExtensionFunction(const char* fName) const
	{
		return vkGetDeviceProcAddr(m_Device, fName);
	}

	void LogicalDevice::CreateQueueIndices()
	{
		uint32_t familyPropCount;
		vkGetPhysicalDeviceQueueFamilyProperties(
			Graphics::Get().GetPhysicalDevice(), &familyPropCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(familyPropCount);
		vkGetPhysicalDeviceQueueFamilyProperties(
			Graphics::Get().GetPhysicalDevice(), &familyPropCount, queueFamilyProperties.data());

		std::optional<uint32_t> graphicsFamily, presentFamily, computeFamily, transferFamily;

		for (uint32_t i = 0; i < familyPropCount; ++i)
		{
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamily = i;
				m_GraphicsFamily = i;
				m_SupportedQueues |= VK_QUEUE_GRAPHICS_BIT;
			}

			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(Graphics::Get().GetPhysicalDevice(), i,
				Graphics::Get().GetSurface(), &presentSupport);

			if (queueFamilyProperties[i].queueCount > 0 && presentSupport)
			{
				presentFamily = i;
				m_PresentFamily = i;
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				computeFamily = i;
				m_ComputeFamily = i;
				m_SupportedQueues |= VK_QUEUE_COMPUTE_BIT;
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				transferFamily = i;
				m_TransferFamily = i;
				m_SupportedQueues |= VK_QUEUE_TRANSFER_BIT;
			}

			// Search until all queues are found
			if (graphicsFamily && presentFamily && computeFamily && transferFamily)
				break;
		}

		if (!graphicsFamily)
			RAY_THROW_RUNTIME("Failed to find graphics family.");
	}

	void LogicalDevice::CreateLogicalDevice()
	{
		auto physicalDeviceFeatures = Graphics::Get().GetPhysicalDevice().GetFeatures();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		float queuePriorities[] = { 0.0f };

		// Fill vector with correct create info for the queues.
		if (m_SupportedQueues & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
			graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueCreateInfo.queueFamilyIndex = m_GraphicsFamily;
			graphicsQueueCreateInfo.queueCount = 1;
			graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(graphicsQueueCreateInfo));
		}
		else
			m_GraphicsFamily = 0;

		if (m_SupportedQueues & VK_QUEUE_COMPUTE_BIT && m_ComputeFamily != m_GraphicsFamily)
		{
			VkDeviceQueueCreateInfo computeQueueCreateInfo{};
			computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			computeQueueCreateInfo.queueFamilyIndex = m_ComputeFamily;
			computeQueueCreateInfo.queueCount = 1;
			computeQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(computeQueueCreateInfo));
		}
		else
			m_ComputeFamily = m_GraphicsFamily;

		if (m_SupportedQueues & VK_QUEUE_TRANSFER_BIT && m_TransferFamily != m_GraphicsFamily &&
			m_TransferFamily != m_ComputeFamily)
		{
			VkDeviceQueueCreateInfo transferQueueCreateInfo{};
			transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			transferQueueCreateInfo.queueFamilyIndex = m_TransferFamily;
			transferQueueCreateInfo.queueCount = 1;
			transferQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(transferQueueCreateInfo));
		}
		else
			m_TransferFamily = m_GraphicsFamily;

		VkPhysicalDeviceFeatures enabledFeatures{};

		// Enable sample rate shading filtering if supported.
		if (physicalDeviceFeatures.sampleRateShading)
			enabledFeatures.sampleRateShading = VK_TRUE;

		// Fill mode non solid is required for wireframe display.
		if (physicalDeviceFeatures.fillModeNonSolid)
			enabledFeatures.fillModeNonSolid = VK_TRUE;

		// Wide lines must be present for line width > 1.0f.
		if (physicalDeviceFeatures.wideLines)
			enabledFeatures.wideLines = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support wireframe pipelines.");

		if (physicalDeviceFeatures.samplerAnisotropy)
			enabledFeatures.samplerAnisotropy = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support sampler anisotropy.");

		if (physicalDeviceFeatures.textureCompressionBC)
			enabledFeatures.textureCompressionBC = VK_TRUE;
		else if (physicalDeviceFeatures.textureCompressionASTC_LDR)
			enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
		else if (physicalDeviceFeatures.textureCompressionETC2)
			enabledFeatures.textureCompressionETC2 = VK_TRUE;

		if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics)
			enabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support vertex pipeline stores and atomics");

		if (physicalDeviceFeatures.fragmentStoresAndAtomics)
			enabledFeatures.fragmentStoresAndAtomics = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support fragment stores and atomics.");

		if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
			enabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support shader storage extended formats.");

		if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
			enabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support shader storage write without "
					  "format.");

		if (physicalDeviceFeatures.geometryShader)
			enabledFeatures.geometryShader = VK_TRUE;
		else
			Log::Warn("[LogicalDeivce] Selected GPU does not support geometry shaders.");

		if (physicalDeviceFeatures.tessellationShader)
			enabledFeatures.tessellationShader = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support tesselation shaders.");

		if (physicalDeviceFeatures.multiViewport)
			enabledFeatures.multiViewport = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support multiple viewports.");

		// Finally create the logical device
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		if (Graphics::Get().GetInstance().ValidationLayersEnabled())
		{
			deviceCreateInfo.enabledLayerCount =
				(uint32_t)VulkanInstance::ValidationLayers().size();
			deviceCreateInfo.ppEnabledLayerNames = VulkanInstance::ValidationLayers().data();
		}

		deviceCreateInfo.enabledExtensionCount = (uint32_t)DeviceExtensions().size();
		deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions().data();
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
		RAY_VK_THROW_FAILED(vkCreateDevice(Graphics::Get().GetPhysicalDevice(), &deviceCreateInfo,
								nullptr, &m_Device),
			"[VulkanLogicalDevice] Failed to create logical device.");

		vkGetDeviceQueue(m_Device, m_GraphicsFamily, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, m_PresentFamily, 0, &m_PresentQueue);
		vkGetDeviceQueue(m_Device, m_ComputeFamily, 0, &m_ComputeQueue);
		vkGetDeviceQueue(m_Device, m_TransferFamily, 0, &m_TransferQueue);
	}
}  // namespace At0::VulkanTesting

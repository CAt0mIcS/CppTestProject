#include "VulkanInstance.h"

#include "Utils/RAssert.h"
#include "Utils/RLogger.h"
#include "Window.h"
#include "Utils/RException.h"

#include <vulkan/vulkan.h>

#define RAY_USE_DEBUG_MESSENGER VK_HEADER_VERSION >= 121


namespace At0::VulkanTesting
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			Log::Warn("[VulkanDebug] {0}", pCallbackData->pMessage);
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			Log::Info("[VulkanDebug] {0}", pCallbackData->pMessage);
		else  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			Log::Error("[VulkanDebug] {0}", pCallbackData->pMessage);

		return VK_FALSE;
	}

	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
		if (func)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (func)
			return func(instance, messenger, pAllocator);
	}

	VulkanInstance::VulkanInstance()
	{
		CreateInstance();
		CreateDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
#if RAY_USE_DEBUG_MESSENGER
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
#endif
		vkDestroyInstance(m_Instance, nullptr);
	}

	void VulkanInstance::FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
		uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
	{
		auto func = reinterpret_cast<PFN_vkCmdPushDescriptorSetKHR>(
			vkGetDeviceProcAddr(device, "vkCmdPushDescriptorSetKHR"));
		if (func)
			func(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount,
				pDescriptorWrites);
	}

	void VulkanInstance::CreateInstance()
	{
		// Default app info
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pEngineName = "Ray";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		// Validation layers aren't required for the program to run, thus we don't throw if we can't
		// find it
		if (m_ValidationLayerEnabled && !CheckValidationLayerSupport())
		{
			Log::Warn("[VulkanInstance] Validation layer support requested but not available");
			m_ValidationLayerEnabled = false;
		}

		auto extensions = GetExtensions();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = (uint32_t)extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

#if RAY_USE_DEBUG_MESSENGER
		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
#endif

		if (m_ValidationLayerEnabled)
		{
#if RAY_USE_DEBUG_MESSENGER
			messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
												  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
												  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
											  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
											  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			messengerCreateInfo.pfnUserCallback = DebugCallback;
			createInfo.pNext = &messengerCreateInfo;
#endif

			createInfo.enabledLayerCount = (uint32_t)s_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = s_ValidationLayers.data();
		}

		RAY_VK_THROW_FAILED(vkCreateInstance(&createInfo, nullptr, &m_Instance),
			"[VulkanInstance] Failed to create Vulkan instance.");
	}

	void VulkanInstance::CreateDebugMessenger()
	{
		if (!m_ValidationLayerEnabled)
			return;

#if RAY_USE_DEBUG_MESSENGER
		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
		messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
											  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
											  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messengerCreateInfo.pfnUserCallback = &DebugCallback;
		RAY_VK_THROW_FAILED(CreateDebugUtilsMessengerEXT(
								m_Instance, &messengerCreateInfo, nullptr, &m_DebugMessenger),
			"[VulkanInstance] Failed to create the Debug Messenger.");
#endif
	}

	bool VulkanInstance::CheckValidationLayerSupport() const
	{
		// Get number of layer properties
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		// Fill the allocated buffer
		std::vector<VkLayerProperties> layerProperties(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

		// Check if reqested layers are found
		for (const char* name : s_ValidationLayers)
		{
			bool layerFound = false;
			for (const VkLayerProperties& layerProps : layerProperties)
			{
				if (strcmp(name, layerProps.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				Log::Warn("[VulkanInstance] The requested layer '{0}' was not found.", name);
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VulkanInstance::GetExtensions() const
	{
		auto [glfwExtensions, glfwExtensionsCount] = Window::Get().GetInstanceExtensions();

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

		if (m_ValidationLayerEnabled)
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}
}  // namespace At0::VulkanTesting

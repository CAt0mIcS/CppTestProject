#include "VulkanInstance.h"
#include "../Utils/RLogger.h"
#include "../Utils/RException.h"
#include "../Utils/RAssert.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	const std::vector<const char*> VulkanInstance::s_EnabledExtensions = {
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};


	VulkanInstance::VulkanInstance()
	{
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledExtensionCount = (uint32_t)s_EnabledExtensions.size();
		createInfo.ppEnabledExtensionNames = s_EnabledExtensions.data();

		RAY_VK_THROW_FAILED(vkCreateInstance(&createInfo, nullptr, &m_Instance),
			"Failed to create Vulkan Instance.");
	}

	VulkanInstance::~VulkanInstance() {}
}  // namespace At0::VulkanTesting

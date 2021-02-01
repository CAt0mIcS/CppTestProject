#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::VulkanTesting
{
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

	private:
		VkInstance m_Instance;

		static const std::vector<const char*> s_EnabledExtensions;
	};
}  // namespace At0::VulkanTesting

#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class LogicalDevice;

	class Renderpass
	{
	public:
		Renderpass(const LogicalDevice* device);
		~Renderpass();

	private:
		const LogicalDevice& m_LogicalDevice;
		VkRenderPass m_Renderpass;
	};
}  // namespace At0::VulkanTesting

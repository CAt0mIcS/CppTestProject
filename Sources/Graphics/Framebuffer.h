#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class LogicalDevice;

	class Framebuffer
	{
	public:
		Framebuffer(const LogicalDevice* device);
		~Framebuffer();

	private:
		VkFramebuffer m_Framebuffer;
		const LogicalDevice& m_LogicalDevice;
	};
}  // namespace At0::VulkanTesting

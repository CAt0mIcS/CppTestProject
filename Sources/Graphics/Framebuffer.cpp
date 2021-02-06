#include "Framebuffer.h"
#include "Graphics/LogicalDevice.h"
#include "Utils/RException.h"


namespace At0::VulkanTesting
{
	Framebuffer::Framebuffer(const LogicalDevice* device) : m_LogicalDevice(*device)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		RAY_VK_THROW_FAILED(
			vkCreateFramebuffer(m_LogicalDevice, &createInfo, nullptr, &m_Framebuffer),
			"Failed to create framebuffer.");
	}

	Framebuffer::~Framebuffer() { vkDestroyFramebuffer(m_LogicalDevice, m_Framebuffer, nullptr); }
}  // namespace At0::VulkanTesting

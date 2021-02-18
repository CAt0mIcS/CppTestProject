#include "pch.h"
#include "Framebuffer.h"
#include "Graphics/Graphics.h"
#include "LogicalDevice.h"
#include "Swapchain.h"
#include "Renderpass/Renderpass.h"


namespace At0::VulkanTesting
{
	Framebuffer::Framebuffer(std::vector<VkImageView> attachments)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = Graphics::Get().GetRenderpass();
		createInfo.attachmentCount = (uint32_t)attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.width = Graphics::Get().GetSwapchain().GetExtent().width;
		createInfo.height = Graphics::Get().GetSwapchain().GetExtent().height;
		createInfo.layers = 1;

		RAY_VK_THROW_FAILED(vkCreateFramebuffer(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_Framebuffer),
			"Failed to create framebuffer.");
	}

	Framebuffer::~Framebuffer()
	{
		vkDestroyFramebuffer(Graphics::Get().GetLogicalDevice(), m_Framebuffer, nullptr);
	}
}  // namespace At0::VulkanTesting

﻿#include "Framebuffer.h"
#include "Graphics/Graphics.h"
#include "Utils/RException.h"
#include "Graphics/Renderpass/Renderpass.h"


namespace At0::VulkanTesting
{
	Framebuffer::Framebuffer(std::vector<VkImageView> attachments)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = Graphics::Get().GetRenderpass();
		createInfo.attachmentCount = attachments.size();
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
		if (m_Framebuffer)
			vkDestroyFramebuffer(Graphics::Get().GetLogicalDevice(), m_Framebuffer, nullptr);
	}
}  // namespace At0::VulkanTesting

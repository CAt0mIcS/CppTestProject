#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

#include "Attachment.h"
#include "Subpass.h"


namespace At0::VulkanTesting
{
	class CommandBuffer;
	class Framebuffer;

	class Renderpass
	{
	public:
		Renderpass(const std::vector<VkAttachmentDescription>& attachments,
			const std::vector<VkSubpassDescription>& subpasses);
		~Renderpass();

		operator const VkRenderPass&() const { return m_Renderpass; }

		void Begin(CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
			const VkClearValue& clearColor) const;
		void End(CommandBuffer& cmdBuff);

	private:
		VkRenderPass m_Renderpass;
	};
}  // namespace At0::VulkanTesting

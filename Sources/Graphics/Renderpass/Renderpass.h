#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

#include "Attachment.h"
#include "Subpass.h"


namespace At0::VulkanTesting
{
	class Renderpass
	{
	public:
		Renderpass(const std::vector<VkAttachmentDescription>& attachments,
			const std::vector<VkSubpassDescription>& subpasses);
		~Renderpass();

		operator const VkRenderPass&() const { return m_Renderpass; }

	private:
		VkRenderPass m_Renderpass;
	};
}  // namespace At0::VulkanTesting

#include "Renderpass.h"
#include "Graphics/Graphics.h"

namespace At0::VulkanTesting
{
	Renderpass::Renderpass(const std::vector<VkAttachmentDescription>& attachments,
		const std::vector<VkSubpassDescription>& subpasses)
	{
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.subpassCount = subpasses.size();
		createInfo.pSubpasses = subpasses.data();

		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;

		RAY_VK_THROW_FAILED(vkCreateRenderPass(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_Renderpass),
			"Failed to create render pass.");
	}

	Renderpass::~Renderpass()
	{
		vkDestroyRenderPass(Graphics::Get().GetLogicalDevice(), m_Renderpass, nullptr);
	}

}  // namespace At0::VulkanTesting

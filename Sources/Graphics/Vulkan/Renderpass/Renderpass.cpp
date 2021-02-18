#include "Renderpass.h"
#include "Graphics/Graphics.h"

#include "Graphics/Vulkan/Framebuffer.h"
#include "Graphics/Vulkan/LogicalDevice.h"
#include "Graphics/Vulkan/Swapchain.h"
#include "Graphics/Vulkan/Commands/CommandBuffer.h"


namespace At0::VulkanTesting
{
	Renderpass::Renderpass(const std::vector<VkAttachmentDescription>& attachments,
		const std::vector<VkSubpassDescription>& subpasses)
	{
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
								  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
								  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask =
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

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

	void Renderpass::Begin(CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
		const std::vector<VkClearValue>& clearColor) const
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Renderpass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = Graphics::Get().GetSwapchain().GetExtent();
		renderPassInfo.clearValueCount = (uint32_t)clearColor.size();
		renderPassInfo.pClearValues = clearColor.data();
		vkCmdBeginRenderPass(cmdBuff, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderpass::End(CommandBuffer& cmdBuff) { vkCmdEndRenderPass(cmdBuff); }

}  // namespace At0::VulkanTesting

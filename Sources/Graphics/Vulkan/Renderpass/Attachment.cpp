#include "Attachment.h"
#include "Graphics/Graphics.h"

namespace At0::VulkanTesting
{
	Attachment::Attachment()
	{
		m_Attachment.format = Graphics::Get().GetSwapchain().GetFormat();
		m_Attachment.samples = VK_SAMPLE_COUNT_1_BIT;

		m_Attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		// Store so we can see the triangle
		m_Attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		// No depth stencil yet
		m_Attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_Attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		// We don't care about the initial layout as we clear the buffer anyways
		m_Attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		// We want the image to be ready for presentation
		m_Attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}
}  // namespace At0::VulkanTesting

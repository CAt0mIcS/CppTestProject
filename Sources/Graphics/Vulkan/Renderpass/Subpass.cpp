#include "Subpass.h"


namespace At0::VulkanTesting
{
	Subpass::Subpass(const std::vector<VkAttachmentReference>& attachmentReferences)
	{
		m_Description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		m_Description.colorAttachmentCount = attachmentReferences.size();
		m_Description.pColorAttachments = attachmentReferences.data();
	}
}  // namespace At0::VulkanTesting

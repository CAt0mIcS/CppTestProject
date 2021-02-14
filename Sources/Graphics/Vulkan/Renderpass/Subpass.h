#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Subpass
	{
	public:
		Subpass(const std::vector<VkAttachmentReference>& attachmentReferences);
		~Subpass() = default;

		const VkSubpassDescription& GetDescription() const { return m_Description; }

	private:
		VkSubpassDescription m_Description{};
	};
}  // namespace At0::VulkanTesting

#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Attachment
	{
	public:
		Attachment();
		~Attachment() = default;
		VkAttachmentDescription GetDescription() const { return m_Attachment; }

	private:
		VkAttachmentDescription m_Attachment{};
	};
}  // namespace At0::VulkanTesting

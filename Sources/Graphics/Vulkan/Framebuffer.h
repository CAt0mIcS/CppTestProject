#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

#include "Base.h"


namespace At0::VulkanTesting
{
	class Renderpass;

	class Framebuffer
	{
	public:
		Framebuffer(std::vector<VkImageView> attachments);
		~Framebuffer();

		operator const VkFramebuffer&() const { return m_Framebuffer; }

	private:
		VkFramebuffer m_Framebuffer;
	};
}  // namespace At0::VulkanTesting

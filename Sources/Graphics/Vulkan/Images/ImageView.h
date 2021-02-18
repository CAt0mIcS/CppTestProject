﻿#pragma once

#include <vulkan/vulkan_core.h>
#include "Base.h"


namespace At0::VulkanTesting
{
	class ImageView
	{
	public:
		ImageView(VkImage image, VkFormat imageFormat, VkImageAspectFlags aspectFlags);
		~ImageView();

		operator const VkImageView&() const { return m_View; }

	private:
		VkImageView m_View;
	};
}  // namespace At0::VulkanTesting

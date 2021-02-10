﻿#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class ImageView
	{
	public:
		ImageView() : m_View(nullptr) {}
		ImageView(VkImage image, VkFormat imageFormat);
		~ImageView();

		operator const VkImageView&() const { return m_View; }

	private:
		VkImageView m_View = nullptr;
	};
}  // namespace At0::VulkanTesting
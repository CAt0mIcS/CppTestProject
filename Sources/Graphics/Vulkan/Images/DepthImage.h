#pragma once

#include "Image.h"


namespace At0::VulkanTesting
{
	class DepthImage : public Image
	{
	public:
		DepthImage();
		VkFormat GetFormat() const { return m_DepthFormat; }
		static bool HasStencilComponent(VkFormat format);

	private:
		VkFormat m_DepthFormat;
	};
}  // namespace At0::VulkanTesting

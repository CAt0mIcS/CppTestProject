#pragma once

#include "Image2D.h"


namespace At0::VulkanTesting
{
	class DepthImage : public Image2D
	{
	public:
		DepthImage();
		~DepthImage() = default;
		VkFormat GetFormat() const { return m_DepthFormat; }
		static bool HasStencilComponent(VkFormat format);

	private:
		VkFormat m_DepthFormat;
	};
}  // namespace At0::VulkanTesting

#pragma once

#include "Drawable.h"


namespace At0::VulkanTesting
{
	class Triangle : public DrawableBase<Triangle>
	{
	public:
		Triangle();
		virtual VkCullModeFlags GetCullMode() const override { return VK_CULL_MODE_BACK_BIT; }
	};
}  // namespace At0::VulkanTesting

#pragma once

#include "Drawable.h"


namespace At0::VulkanTesting
{
	class Cube : public DrawableBase<Cube>
	{
	public:
		Cube();
		virtual VkCullModeFlags GetCullMode() const override { return VK_CULL_MODE_FRONT_BIT; }
	};
}  // namespace At0::VulkanTesting

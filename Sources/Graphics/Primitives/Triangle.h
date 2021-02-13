#pragma once

#include "Drawable.h"


namespace At0::VulkanTesting
{
	class Triangle : public DrawableBase<Triangle>
	{
	public:
		Triangle();
		~Triangle() = default;
	};
}  // namespace At0::VulkanTesting

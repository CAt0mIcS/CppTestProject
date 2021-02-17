#pragma once

#include "Drawable.h"

#include <vector>


namespace At0::VulkanTesting
{
	class Mesh : public Drawable
	{
	public:
		Mesh(std::vector<Ref<Bindable>> bindables);
	};
}  // namespace At0::VulkanTesting

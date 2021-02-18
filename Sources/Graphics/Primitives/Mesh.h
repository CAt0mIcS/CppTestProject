#pragma once

#include "Base.h"
#include "Drawable.h"

#include <vector>


namespace At0::VulkanTesting
{
	class Mesh : public Drawable
	{
	public:
		Mesh(std::vector<Ref<Bindable>> bindables);
		void CmdDraw(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

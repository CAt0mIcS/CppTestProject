#pragma once

#include "Drawable.h"
#include "Base.h"

namespace At0::VulkanTesting
{
	class Square : public Drawable
	{
	public:
		Square();
		void CmdBind(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

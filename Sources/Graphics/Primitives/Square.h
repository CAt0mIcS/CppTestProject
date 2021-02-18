#pragma once

#include "Drawable.h"

namespace At0::VulkanTesting
{
	class Square : public Drawable
	{
	public:
		Square();
		void CmdDraw(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

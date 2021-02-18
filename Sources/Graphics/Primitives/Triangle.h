#pragma once

#include "Drawable.h"
#include "Base.h"


namespace At0::VulkanTesting
{
	class Triangle : public Drawable
	{
	public:
		Triangle();
		void CmdDraw(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

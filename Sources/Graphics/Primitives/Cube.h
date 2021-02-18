#pragma once

#include "Base.h"
#include "Drawable.h"


namespace At0::VulkanTesting
{
	class Cube : public Drawable
	{
	public:
		Cube();
		void CmdDraw(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

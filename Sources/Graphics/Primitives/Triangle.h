#pragma once

#include "Drawable.h"
#include "Base.h"


namespace At0::VulkanTesting
{
	class Triangle : public Drawable
	{
	public:
		Triangle();
		void CmdBind(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

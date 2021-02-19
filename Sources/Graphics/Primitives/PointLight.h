#pragma once

#include "Drawable.h"
#include "Base.h"

namespace At0::VulkanTesting
{
	class PointLight : public Drawable
	{
	public:
		PointLight();
		virtual void CmdBind(const CommandBuffer& cmdBuff) override;
	};
}  // namespace At0::VulkanTesting

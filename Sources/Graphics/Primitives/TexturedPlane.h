#pragma once

#include "Drawable.h"
#include "Graphics/Vulkan/Images/Image2D.h"
#include "Base.h"


namespace At0::VulkanTesting
{
	class TexturedPlane : public Drawable
	{
	public:
		TexturedPlane();
		virtual void CmdBind(const CommandBuffer& cmdBuff) override;

	private:
		Image2D m_Texture;
	};
}  // namespace At0::VulkanTesting

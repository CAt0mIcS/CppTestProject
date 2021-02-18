#pragma once

#include "Drawable.h"
#include "Graphics/Vulkan/Images/Image.h"
#include "Base.h"


namespace At0::VulkanTesting
{
	class TexturedPlane : public Drawable
	{
	public:
		TexturedPlane();
		virtual void CmdDraw(const CommandBuffer& cmdBuff) override;

	private:
		Image m_Texture;
	};
}  // namespace At0::VulkanTesting

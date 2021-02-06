#include "RenderPass.h"
#include "Utils/RException.h"
#include "Graphics/LogicalDevice.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Renderpass::Renderpass(const LogicalDevice* device) : m_LogicalDevice(*device)
	{
		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		RAY_VK_THROW_FAILED(
			vkCreateRenderPass(m_LogicalDevice, &createInfo, nullptr, &m_Renderpass),
			"Failed to create render pass.");
	}

	Renderpass::~Renderpass() { vkDestroyRenderPass(m_LogicalDevice, m_Renderpass, nullptr); }
}  // namespace At0::VulkanTesting

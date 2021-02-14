#include "CommandPool.h"
#include "Graphics/Graphics.h"
#include "Utils/RException.h"


namespace At0::VulkanTesting
{
	CommandPool::CommandPool()
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.queueFamilyIndex = Graphics::Get().GetLogicalDevice().GetGraphicsFamily();

		RAY_VK_THROW_FAILED(vkCreateCommandPool(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_CommandPool),
			"Failed to create a command buffer");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(Graphics::Get().GetLogicalDevice(), m_CommandPool, nullptr);
	}
}  // namespace At0::VulkanTesting

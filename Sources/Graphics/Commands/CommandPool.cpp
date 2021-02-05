#include "CommandPool.h"
#include "../Graphics.h"
#include "../LogicalDevice.h"
#include "../../Utils/RException.h"


namespace At0::VulkanTesting
{
	CommandPool::CommandPool(const LogicalDevice* device) : m_LogicalDevice(*device)
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags =
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = m_LogicalDevice.GetGraphicsFamily();

		RAY_VK_THROW_FAILED(
			vkCreateCommandPool(m_LogicalDevice, &createInfo, nullptr, &m_CommandPool),
			"Failed to create a command buffer");
	}

	CommandPool::~CommandPool() { vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr); }
}  // namespace At0::VulkanTesting
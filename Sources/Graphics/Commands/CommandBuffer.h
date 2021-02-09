#pragma once

#include <vulkan/vulkan_core.h>
#include <memory>


namespace At0::VulkanTesting
{
	class CommandBuffer
	{
	public:
		CommandBuffer();
		~CommandBuffer();

		void Begin();
		void End();

		operator const VkCommandBuffer&() const { return m_CommandBuffer; }

	private:
		VkCommandBuffer m_CommandBuffer;
		bool m_Running = false;
	};
}  // namespace At0::VulkanTesting

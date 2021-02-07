#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Shader;

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(Shader&& shader);
		~GraphicsPipeline();

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;
	};
}  // namespace At0::VulkanTesting

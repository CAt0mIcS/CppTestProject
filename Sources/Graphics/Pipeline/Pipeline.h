#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Shader;
	class Renderpass;

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(Shader&& shader, const Renderpass& renderpass);
		~GraphicsPipeline();

		operator const VkPipeline&() const { return m_Pipeline; }

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;
	};
}  // namespace At0::VulkanTesting
